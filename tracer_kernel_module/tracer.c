// SPDX-License-Identifier: GPL-2.0+

/*
 * tracer
 *
 * Author:
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/ioctl.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/atomic.h>
#include "tracer.h"

#define PROCFS_MAX_SIZE		512
#define procfs_file_read	"tracer"

// pointer to the tracer proc entry
struct proc_dir_entry *proc_tracer_read;

/*
	miscdevice used by this module to add/remove
	traced processes through an ioctl interface
*/
static struct miscdevice my_device;

// adress of kmalloc and size
struct kmallocs_mem_list {
	unsigned int address;
	unsigned int size;
	struct list_head list;
};

/* 
	for each observed process
	count calls count
	store addresses and sizes of kmallocs
*/
struct observed_processes_list
{
	unsigned int pid;
	atomic_t kmalloc_calls;
	atomic_t kfree_calls;
	atomic_t kmalloc_mem;
	atomic_t kfree_mem;
	atomic_t sched_calls;
	atomic_t up_calls;
	atomic_t down_calls;
	atomic_t lock_calls;
	atomic_t unlock_calls;
	struct list_head list;
	struct kmallocs_mem_list kmallocs_list;
};

struct observed_processes_list processes_list = {
	.pid = 0,
	.list = LIST_HEAD_INIT(processes_list.list)
};



// get process struct from list:
static struct observed_processes_list *get_process(unsigned int pid)
{
	struct observed_processes_list *elem;
	struct list_head *pos, *aux;

	list_for_each_safe(pos, aux, &processes_list.list)
	{
		elem = list_entry(pos, struct observed_processes_list, list);
		//search for process with given pid
		if(elem->pid == pid)
		{
			return elem;
		}
	}

	return NULL;
}

// remove process from the observed processes list:
static void remove_process(unsigned int pid)
{
	struct observed_processes_list *elem;
	struct list_head *pos, *aux;

	list_for_each_safe(pos, aux, &processes_list.list)
	{
		elem = list_entry(pos, struct observed_processes_list, list);
		//search for process with given pid
		if(elem->pid == pid)
		{
			// TODO: delete kmallocs list
			list_del(pos);
			kfree(elem);
		}
	}
}

// add a process to the observed processes list
static void add_process(unsigned int pid)
{
	struct observed_processes_list *list_elem;

	// add a new cell for the process
	list_elem = kmalloc(sizeof(struct observed_processes_list), GFP_KERNEL);
	memset(list_elem, 0, sizeof(struct observed_processes_list));

	if(!list_elem)
		pr_alert("kmalloc err in add_process(pid)");
	list_elem->pid = pid;
	INIT_LIST_HEAD(&list_elem->list);

	// init proc list head:
	INIT_LIST_HEAD(&list_elem->kmallocs_list.list);

	list_add(&list_elem->list, &processes_list.list);
}

// add kmalloc address and size to process:
static void add_kmalloc_address(
	struct observed_processes_list *process,
	unsigned int address,
	unsigned int size
)
{
	struct kmallocs_mem_list *list_elem;
	// add a new cell for the process
	list_elem = kmalloc(sizeof(struct kmallocs_mem_list), GFP_ATOMIC);
	memset(list_elem, 0, sizeof(struct kmallocs_mem_list));

	if(!list_elem)
		pr_alert("kmalloc err in add_kmalloc_address");
	list_elem->address = address;
	list_elem->size = size;
	INIT_LIST_HEAD(&list_elem->list);

	list_add(&list_elem->list, &process->kmallocs_list.list);
}

// remove address from kmalloc address process list
// return size of allocated mem
static unsigned int remove_kmalloc_address(
	struct observed_processes_list *process,
	unsigned int address)
{
	struct kmallocs_mem_list *elem;
	struct list_head *pos, *aux;
	unsigned int alloc_size;

	// search for address, remove it from list, return kmalloc size
	list_for_each_safe(pos, aux, &process->kmallocs_list.list)
	{
		elem = list_entry(pos, struct kmallocs_mem_list, list);
		if(elem->address == address)
		{
			list_del(pos);
			alloc_size = elem->size;
			kfree(elem);
			return alloc_size;
		}
	}
	return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
	    case TRACER_ADD_PROCESS:
	        /* add process to the list */
	        add_process(arg);
	        break;
	    case TRACER_REMOVE_PROCESS:
	    	/* remove process from list */
	        remove_process(arg);
	    	break;
	    default:
	        return -ENOTTY;
    }
 
    return 0;
}

// print stats of processes (call counts/mem alloc.)
static int tracer_proc_show(struct seq_file *m, void *v)
{
	struct list_head *pos;
	struct observed_processes_list *elem;
	char buf[200];
	seq_puts(m, "PID   kmalloc kfree kmalloc_mem kfree_mem  sched   up     down  lock   unlock\n");
	list_for_each(pos, &processes_list.list)
	{
		elem = list_entry(pos, struct observed_processes_list, list);
		sprintf(buf, "%ld %d %d %d %d %d %d %d %d %d\n",
			elem->pid,
			atomic_read(&elem->kmalloc_calls),
			atomic_read(&elem->kfree_calls),
			atomic_read(&elem->kmalloc_mem),
			atomic_read(&elem->kfree_mem),
			atomic_read(&elem->sched_calls),
			atomic_read(&elem->up_calls),
			atomic_read(&elem->down_calls),
			atomic_read(&elem->lock_calls),
			atomic_read(&elem->unlock_calls)
			);
		seq_puts(m, buf);
	}

	return 0;
}

static int tracer_read_open(struct inode *inode, struct  file *file) 
{
	return single_open(file, tracer_proc_show, NULL);
}

/* per-instance private data */
struct my_data
{
	unsigned int kmalloc_size;
};

/* 
	kretprobe handlers:
	when hit: 
		- will count calls
		- will add/remove from process kmalloc list adresses
*/
static int kmalloc_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	
	elem = get_process(pid);

	if (!elem) {
		return 0;
	}
	
	// increment nr calls:
	atomic_inc(&elem->kmalloc_calls);

	// save in instance data size of alloc:
	data = (struct my_data *)ri->data;

	// size of alloc resides in first register
	data->kmalloc_size = regs->ax;

	// add size of kmalloc to current process mem size:
	atomic_add(data->kmalloc_size, &elem->kmalloc_mem);

	return 0;
}


static int kmalloc_return_handler(struct kretprobe_instance *instance, struct pt_regs *regs) 
{
	struct my_data *data;
	struct observed_processes_list *elem;

	// identify current task
	int pid = task_pid_nr(current);
	unsigned int returned_address;
	elem = get_process(pid);

	if (!elem) {
		// task not traced
		return 0;
	}

	// get returned address by kmalloc:
	returned_address = regs_return_value(regs);

	// add address to list:
	data = (struct my_data *)instance->data;
	add_kmalloc_address(elem, returned_address, data->kmalloc_size);
	return 0;
}

static int kfree_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	unsigned int address;
	unsigned int size;
	elem = get_process(pid);

	if (!elem) {
		return 0;
	}
	
	// increment nr calls:
	atomic_inc(&elem->kfree_calls);

	// get size of allocated mem at this address:
	size = remove_kmalloc_address(elem, regs->ax);
	if (!size) {
		// no mem allocated at this address:
		return 0;
	}

	// add size of kfree to current process mem size:
	atomic_add(size, &elem->kfree_mem);
	return 0;
}

static int empty_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	return 0;
}

static int schedule_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	
	elem = get_process(pid);

	if (!elem) {
		return 0;
	}
	
	// increment nr calls:
	atomic_inc(&elem->sched_calls);
	return 0;
}

static int up_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	elem = get_process(pid);
	if (!elem) {
		return 0;
	}
	
	// increment nr calls:
	atomic_inc(&elem->up_calls);
	return 0;
}

static int down_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	elem = get_process(pid);
	if (!elem) {
		return 0;
	}
	// increment nr calls:
	atomic_inc(&elem->down_calls);
	return 0;
}

static int mutex_lock_probe_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	elem = get_process(pid);
	if (!elem) {
		return 0;
	}
	// increment nr calls:
	atomic_inc(&elem->lock_calls);
	return 0;
}

static int mutex_unlock_probe_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct observed_processes_list *elem;
	int pid = task_pid_nr(current);
	elem = get_process(pid);
	if (!elem) {
		return 0;
	}
	// increment nr calls:
	atomic_inc(&elem->unlock_calls);
	return 0;
}

// kretprobes:
static struct kretprobe kmalloc_probe = {                                                                                                                                                          
    .handler = kmalloc_return_handler, /* return probe handler */
	.entry_handler = kmalloc_entry_handler,
    .maxactive = 32,
    .kp.symbol_name = "__kmalloc"
};

static struct kretprobe kfree_probe = {
	.entry_handler = kfree_entry_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "kfree"
};

static struct kretprobe schedule_probe = {
	.entry_handler = schedule_entry_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "schedule"
};

static struct kretprobe up_probe = {
	.entry_handler = up_entry_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "up"
};

static struct kretprobe down_probe = {
	.entry_handler = down_entry_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "down_interruptible"
};

static struct kretprobe mutex_lock_probe = {
	.entry_handler = mutex_lock_probe_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "mutex_lock_nested"
};

static struct kretprobe mutex_unlock_probe = {
	.entry_handler = mutex_unlock_probe_handler,
	.handler = empty_handler,
    .maxactive = 32,
    .kp.symbol_name = "mutex_unlock"
};



// device file operations:
static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl
};

// tracer proc file operations:
static const struct file_operations r_fops = {
	.owner		= THIS_MODULE,
	.open		= tracer_read_open,
	.read		= seq_read,
	.release	= single_release,
};



static int tracer_init(void) 
{
	int err = 0;
	
	// register miscdevice with major 10 and minor 42
    my_device.minor = TRACER_DEV_MINOR;
    my_device.name = TRACER_DEV_NAME;
    my_device.fops = &my_fops;
    err = misc_register(&my_device);
    if (err) {
    	pr_alert("Tracer misc_register err.");
    	return 1;
    }

    // register kretprobes on function calls:
    err = register_kretprobe(&kmalloc_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(kmalloc_probe) err.");
		return 1;
	}
	
	err = register_kretprobe(&kfree_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(kfree_probe) err.");
		return 1;
	}

	err = register_kretprobe(&schedule_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(schedule_probe) err.");
		return 1;
	}

	err = register_kretprobe(&up_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(up_probe) err.");
		return 1;
	}

	err = register_kretprobe(&down_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(down_probe) err.");
		return 1;
	}

	err = register_kretprobe(&mutex_lock_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(mutex_lock_probe) err.");
		return 1;
	}

	err = register_kretprobe(&mutex_unlock_probe);
	if (err < 0) {
		pr_alert("Tracer register_kretprobe(mutex_unlock_probe) err.");
		return 1;
	}

    // create /proc/tracer viewer file:
    proc_tracer_read = proc_create(procfs_file_read, 0000, NULL, &r_fops);
	if (!proc_tracer_read) {
		pr_alert("Tracer proc_create err.");
		return -ENOMEM;
	}

	return 0;
}

static void tracer_exit(void) 
{
	// remove miscdevice
	misc_deregister(&my_device);
	
	// remove proc tracer viewer file
	proc_remove(proc_tracer_read);

	// deregister kprobes and kretprobes
	unregister_kretprobe(&kmalloc_probe);
	unregister_kretprobe(&kfree_probe);
	unregister_kretprobe(&schedule_probe);
	unregister_kretprobe(&up_probe);
	unregister_kretprobe(&down_probe);
	unregister_kretprobe(&mutex_lock_probe);
	unregister_kretprobe(&mutex_unlock_probe);

}

module_init(tracer_init);
module_exit(tracer_exit);

MODULE_DESCRIPTION("Linux kernel tracer module.");
MODULE_AUTHOR("");
MODULE_LICENSE("GPL v2");
