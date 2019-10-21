#include "ssr.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/genhd.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/vmalloc.h>
#include <linux/crc32.h>

MODULE_DESCRIPTION("Tema 3 - Software RAID");
MODULE_AUTHOR("Sandulescu Andreea");
MODULE_LICENSE("GPL");

static struct block_device *phys_bdev_vdb;
static struct block_device *phys_bdev_vdc;
static struct workqueue_struct *ssr_workqueue;

struct my_device_data {
    struct work_struct my_work;
    struct bio *bio;
};

static struct ssr_block_dev {
	spinlock_t lock;
	struct request_queue *queue;
	struct gendisk *gd;
	u8 *data;
	size_t size;
} g_dev;

static int ssr_block_open(struct block_device *bdev, fmode_t mode)
{
	return 0;
}

static void ssr_block_release(struct gendisk *gd, fmode_t mode)
{
}

static const struct block_device_operations ssr_block_ops = {
	.owner = THIS_MODULE,
	.open = ssr_block_open,
	.release = ssr_block_release
};

static struct block_device *open_disk(char *name)
{
	struct block_device *bdev;
	
	bdev = blkdev_get_by_path(name, FMODE_READ | FMODE_WRITE | FMODE_EXCL, THIS_MODULE);

	if (IS_ERR(bdev))
		printk (KERN_NOTICE "[open_disk]Err for disk %s!\n", name);
	return bdev;
}

static void close_disk(struct block_device *bdev)
{
	blkdev_put(bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
}

//create a new bio structure with one page
static struct bio* create_bio_one_page(sector_t sect, unsigned int op, struct gendisk* disk, struct page **new_page)
{
	struct page *page = alloc_page(GFP_NOIO);
    struct bio *new_bio = bio_alloc(GFP_NOIO , 1);

	new_bio->bi_disk = disk;
	new_bio->bi_iter.bi_sector = sect;
	new_bio->bi_opf = op;

  	bio_add_page(new_bio, page, KERNEL_SECTOR_SIZE, 0);
  	*new_page = page;
    return new_bio;
}

static struct bio* create_bio(struct bio *my_blkdev_bio, struct gendisk* disk)
{
	struct bio_vec bvec;
    struct bvec_iter i;
    struct bio *new_bio = bio_alloc(GFP_NOIO , my_blkdev_bio->bi_vcnt);

	new_bio->bi_disk = disk;
	new_bio->bi_iter.bi_sector = my_blkdev_bio->bi_iter.bi_sector;;
	new_bio->bi_opf = my_blkdev_bio->bi_opf;

	bio_for_each_segment(bvec, my_blkdev_bio, i) {
      	bio_add_page(new_bio, bvec.bv_page, bvec.bv_len, bvec.bv_offset);
    }

    return new_bio;
}

static char* read_a_sector(sector_t sector, struct gendisk* disk, char*buf)
{
	char *bio_buf;
	struct page *page; 
	struct bio* bio = create_bio_one_page(sector, REQ_OP_READ, disk, &page);

	submit_bio_wait(bio);

	bio_buf = kmap(page);
	memcpy(buf, bio_buf, KERNEL_SECTOR_SIZE);
 	
 	kunmap(page);
	bio_put(bio);
	__free_page(page);

	return buf;
}

static void write_a_sector(sector_t sector, struct gendisk* disk, char* buf)
{
	char *bio_buf;
	struct page *page; 
	struct bio* bio = create_bio_one_page(sector, REQ_OP_WRITE, disk, &page);

	bio_buf = kmap(page);
	memcpy(bio_buf, buf, KERNEL_SECTOR_SIZE);

	kunmap(page);

	submit_bio_wait(bio);

	bio_put(bio);
	__free_page(page);
}

//reads the crc for one sector and returns it
static u32 read_crc(sector_t sector, struct gendisk* disk)
{
	int crc_pos, crc_sector, crc_offset;
	struct bio* bio;
	struct page *page;
	char *buf;
	u32 crc_val;

	crc_pos = LOGICAL_DISK_SIZE + sizeof(u32) * sector;
	crc_sector = crc_pos / KERNEL_SECTOR_SIZE;
	crc_offset = crc_pos - crc_sector * KERNEL_SECTOR_SIZE;

	bio = create_bio_one_page(crc_sector, REQ_OP_READ, disk, &page);
	submit_bio_wait(bio);

	buf = kmap(page);
	memcpy(&crc_val, buf + crc_offset, sizeof(u32));

	kunmap(page);
	bio_put(bio);
	__free_page(page);

	return crc_val;
}

static bool make_consistent(struct gendisk* disk_vdb, struct gendisk* disk_vdc, struct bio *bio)
{
	char *vdb_buf, *vdc_buf;
	int i = 0, init_sector = bio->bi_iter.bi_sector;
	int cnt_sectors = bio_sectors(bio);
	bool corrupted = false;

	vdb_buf = kmalloc(KERNEL_SECTOR_SIZE, GFP_KERNEL);
	if(!vdb_buf)
		printk (KERN_NOTICE "[make_consistent]Err in kmalloc!\n");

	vdc_buf = kmalloc(KERNEL_SECTOR_SIZE, GFP_KERNEL);
	if(!vdc_buf)
		printk (KERN_NOTICE "[make_consistent]Err in kmalloc!\n");

    for(i = init_sector; i < init_sector + cnt_sectors; i++)
    {
    	u32 comp_crc_val_vdb, comp_crc_val_vdc, crc_val_vdb, crc_val_vdc;

    	vdb_buf = read_a_sector(i, disk_vdb, vdb_buf);
    	comp_crc_val_vdb = crc32(0, vdb_buf, KERNEL_SECTOR_SIZE);
    	crc_val_vdb = read_crc(i, disk_vdb);

    	vdc_buf = read_a_sector(i, disk_vdc, vdc_buf);
    	comp_crc_val_vdc = crc32(0, vdc_buf, KERNEL_SECTOR_SIZE);
    	crc_val_vdc = read_crc(i, disk_vdc);

    	if(comp_crc_val_vdb == crc_val_vdb)
    	{
    		if(comp_crc_val_vdc != crc_val_vdc)
	    		write_a_sector(i, disk_vdc, vdb_buf);
    	}
    	else
    	{
    		if(comp_crc_val_vdc == crc_val_vdc)
	    		write_a_sector(i, disk_vdb, vdc_buf);
	    	else
	    	{
	    		corrupted = true;
	    		break;
	    	}
    	}
    }

    kfree(vdb_buf);
    kfree(vdc_buf);
    return corrupted;
}

static void update_crc(struct gendisk* disk, struct bio *bio)
{
	int i = 0, init_sector = bio->bi_iter.bi_sector;
	int crc_pos = 0, crc_sector, crc_offset;
	char *buf, *crc_buf;
    int cnt_sectors = bio_sectors(bio);

    //compute crc values
    for(i = init_sector; i < init_sector + cnt_sectors; i++)
    {
    	u32 crc_val;

    	struct bio *aux_bio, *new_bio, *crc_bio;
    	struct page *new_page, *crc_page, *page = alloc_page(GFP_NOIO);

    	crc_bio = bio_alloc(GFP_NOIO, 1);
    	crc_bio->bi_disk = disk;
		crc_bio->bi_iter.bi_sector = i;
		crc_bio->bi_opf = REQ_OP_READ;

		bio_add_page(crc_bio, page, KERNEL_SECTOR_SIZE, 0);
		submit_bio_wait(crc_bio);

		buf = kmap(page);
		crc_val = crc32(0, buf, KERNEL_SECTOR_SIZE);

	 	kunmap(page);
	 	bio_put(crc_bio);
	 	__free_page(page);

		crc_pos = LOGICAL_DISK_SIZE + sizeof(u32) * i;
		crc_sector = crc_pos / KERNEL_SECTOR_SIZE;
		crc_offset = crc_pos - crc_sector * KERNEL_SECTOR_SIZE;

		new_bio = create_bio_one_page(crc_sector, REQ_OP_READ, disk, &new_page);
		submit_bio_wait(new_bio);

		buf = kmap(new_page);
		memcpy(buf + crc_offset, &crc_val, sizeof(u32));

		aux_bio = create_bio_one_page(crc_sector, REQ_OP_WRITE, disk, &crc_page);
		crc_buf = kmap(crc_page);
		memcpy(crc_buf, buf, KERNEL_SECTOR_SIZE);

		kunmap(crc_page);
		kunmap(new_page);
	 	bio_put(new_bio);
		
		submit_bio_wait(aux_bio);

	 	__free_page(crc_page);
	 	__free_page(new_page);
		
	 	bio_put(aux_bio);
    }

}

void my_work_handler(struct work_struct *work) {

  	int err;
  	bool corrupted = false;
    struct my_device_data *my_data = container_of(work, struct my_device_data, my_work);
    struct bio *my_blkdev_bio = my_data->bio;
    struct bio *vdb_bio = create_bio(my_blkdev_bio, phys_bdev_vdb->bd_disk);
    struct bio *vdc_bio = create_bio(my_blkdev_bio, phys_bdev_vdc->bd_disk);

	if (bio_data_dir(my_blkdev_bio) == WRITE)
	{
		err = submit_bio_wait(vdb_bio);
		if(err != 0)
			printk (KERN_NOTICE "[my_work_handler]Err at submit_bio_wait, vdb!\n");

		err = submit_bio_wait(vdc_bio);
		if(err != 0)
			printk (KERN_NOTICE "[my_work_handler]Err at submit_bio_wait, vdc!\n");

		update_crc(phys_bdev_vdb->bd_disk, my_blkdev_bio);
		update_crc(phys_bdev_vdc->bd_disk, my_blkdev_bio);

	}
	else if(bio_data_dir(my_blkdev_bio) == READ)
	{
		corrupted = make_consistent(vdb_bio->bi_disk, vdc_bio->bi_disk, my_blkdev_bio);
		if(!corrupted)
		{
			err = submit_bio_wait(vdb_bio);
			if(err != 0)
				printk (KERN_NOTICE "[my_work_handler]Err at submit_bio_wait, vdb!\n");

			err = submit_bio_wait(vdc_bio);
			if(err != 0)
				printk (KERN_NOTICE "[my_work_handler]Err at submit_bio_wait, vdc!\n");
		}
	}

	if(!corrupted)
		bio_endio(my_blkdev_bio);
	else
		bio_io_error(my_blkdev_bio);
    bio_put(vdb_bio);
    bio_put(vdc_bio);
}

static unsigned int ssr_make_request(struct request_queue *q, struct bio *bio)
{
	struct my_device_data* my_dev_data;

	my_dev_data = kmalloc(sizeof (struct my_device_data), GFP_ATOMIC);
	if(!my_dev_data)
		printk (KERN_NOTICE "[ssr_make_request]Err in kmalloc!\n");

	my_dev_data->bio = bio;
	INIT_WORK(&my_dev_data->my_work, my_work_handler);
	queue_work(ssr_workqueue, &my_dev_data->my_work);
	return 0;
}

static int create_block_device(struct ssr_block_dev *dev)
{
	int err;

	dev->size = 0;
	dev->data = NULL;
	/* initialize the I/O queue */
	spin_lock_init(&dev->lock);

	dev->queue = blk_alloc_queue(GFP_KERNEL);
	if (dev->queue == NULL) {
        printk (KERN_ERR "[create_block_device]Cannot allocate block device queue\n");
        err = -ENOMEM;
		goto out_blk_init;
	}

	blk_queue_make_request(dev->queue, ssr_make_request);
	blk_queue_logical_block_size(dev->queue, KERNEL_SECTOR_SIZE);
	dev->queue->queuedata = dev;

	/* initialize the gendisk structure */
	dev->gd = alloc_disk(SSR_NUM_MINORS);
	if (!dev->gd) {
		printk(KERN_ERR "[create_block_device]alloc_disk: failure\n");
		err = -ENOMEM;
		goto out_alloc_disk;
	}

	dev->gd->major = SSR_MAJOR;
	dev->gd->first_minor = SSR_FIRST_MINOR;
	dev->gd->fops = &ssr_block_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	snprintf(dev->gd->disk_name, DISK_NAME_LEN, LOGICAL_DISK_NAME);
	set_capacity(dev->gd, LOGICAL_DISK_SECTORS);

	add_disk(dev->gd);

	return 0;

out_alloc_disk:
	blk_cleanup_queue(dev->queue);
out_blk_init:
	vfree(dev->data);
	return err;
}

static int __init ssr_block_init(void)
{
	int err = 0;
	ssr_workqueue = create_singlethread_workqueue("my_wq");

	err = register_blkdev(SSR_MAJOR, LOGICAL_DISK_NAME);
    if (err < 0) {
    	printk(KERN_ERR "[ssr_block_init]unable to register %s block device\n", LOGICAL_DISK_NAME);
        goto out;
    }

    err = create_block_device(&g_dev);
    if (err < 0) {
        printk(KERN_ERR "[ssr_block_init]unable to register %s block device\n", LOGICAL_DISK_NAME);
        goto out;
    }

	phys_bdev_vdb = open_disk(PHYSICAL_DISK1_NAME);
	if (phys_bdev_vdb == NULL) {
		printk(KERN_ERR "[ssr_block_init] No such device (%s)\n", PHYSICAL_DISK1_NAME);
		err = -EINVAL;
		goto out;
	}

	phys_bdev_vdc = open_disk(PHYSICAL_DISK2_NAME);
	if (phys_bdev_vdc == NULL) {
		printk(KERN_ERR "[ssr_block_init] No such device (%s)\n", PHYSICAL_DISK2_NAME);
		err = -EINVAL;
		close_disk(phys_bdev_vdb);
		goto out;
	}

	return 0;

out:
	unregister_blkdev(SSR_MAJOR, LOGICAL_DISK_NAME);
	return err;
}

static void delete_block_device(struct ssr_block_dev *dev)
{
	if (dev->gd) {
		del_gendisk(dev->gd);
		put_disk(dev->gd);
	}
	if (dev->queue)
		blk_cleanup_queue(dev->queue);
	if (dev->data)
		vfree(dev->data);
}

static void __exit ssr_block_exit(void)
{
	flush_workqueue(ssr_workqueue);
	destroy_workqueue(ssr_workqueue);

	delete_block_device(&g_dev);

	unregister_blkdev(SSR_MAJOR, LOGICAL_DISK_NAME);

	close_disk(phys_bdev_vdb);
	close_disk(phys_bdev_vdc);
}

module_init(ssr_block_init);
module_exit(ssr_block_exit);
