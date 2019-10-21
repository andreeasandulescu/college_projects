// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * Author: Andreea Sandulescu <andreea.sandulescu@stud.acs.upb.ro>
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

#define PROCFS_MAX_SIZE		512

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

struct my_list
{
	char *str;
	struct list_head list;
};

struct my_list str_list = 
{
	.str = NULL,
	.list = LIST_HEAD_INIT(str_list.list)
};

/*
function used for adding an new cell to the list
the parameter 'c' is set to the letter 'e' if the 
element should be added to the end of the list, 
otherwise it will be added to the beginning
*/
static void add_to_list(char* buffer, char c)
{
	char *my_str, *p = buffer;
	struct my_list *list_elem;
	struct list_head *head;

	//pointer to the string that is of interest
	strsep(&p," ");

	my_str = kmalloc(strlen(p) + 1, GFP_KERNEL);
	if(!my_str)
		pr_alert("kmalloc err in add_to_beginning(my_str)");

	strcpy(my_str, p);

	list_elem = kmalloc(sizeof(struct my_list), GFP_KERNEL);
	if(!list_elem)
		pr_alert("kmalloc err in add_to_beginning(list_elem)");
	list_elem->str = my_str;
	INIT_LIST_HEAD(&list_elem->list);

	head = &str_list.list;
	//add to the end
	if(c == 'e')
		head = head->prev;;

	list_add(&list_elem->list, head); 
}

static void rem_from_list(char* buffer, char c)
{
	char *p = buffer;
	struct my_list *elem;
	struct list_head *pos, *aux;

	//pointer to the string that is of interest
	strsep(&p," ");

	list_for_each_safe(pos, aux, &str_list.list)
	{
		elem = list_entry(pos, struct my_list, list);
		//search for elements containing the string given as parameter
		if(elem->str != NULL && strcmp(p, elem->str) == 0)
		{
			kfree(elem->str);
			list_del(pos);
			kfree(elem);

			if(c == 'o')	//del one element
				return;
		}
	}
}

static int list_proc_show(struct seq_file *m, void *v)
{
	struct list_head *pos;
	struct my_list *elem;
	list_for_each(pos, &str_list.list)
	{
		elem = list_entry(pos, struct my_list, list);
		seq_puts(m, elem->str);
	}

	return 0;
}

static int list_read_open(struct inode *inode, struct  file *file) 
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
			  size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	if(strncmp(local_buffer, "add", 3) == 0)
	{
		if(strncmp(local_buffer, "addf", 4) == 0)
			add_to_list(local_buffer,'x');
		else
			add_to_list(local_buffer,'e');

	}
	else
	{
		if(strncmp(local_buffer, "del", 3) == 0)
		{
			if(strncmp(local_buffer, "delf", 4) == 0)
				rem_from_list(local_buffer,'o');
			else
				rem_from_list(local_buffer,'x');

		}
		else
			pr_alert("Invalid command");
	}

	return local_buffer_size;
}

static const struct file_operations r_fops = {
	.owner		= THIS_MODULE,
	.open		= list_read_open,
	.read		= seq_read,
	.release	= single_release,
};

static const struct file_operations w_fops = {
	.owner		= THIS_MODULE,
	.open		= list_write_open,
	.write		= list_write,
	.release	= single_release,
};

static int list_init(void)
{
	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
				     &r_fops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
				      &w_fops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");

MODULE_AUTHOR("Andreea Sandulescu");
MODULE_LICENSE("GPL v2");
