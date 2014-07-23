#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define DEBUG if(flexnproc_verbose >= 1)
#define FLEX_PROC_ENTRY "flexnproc"

static unsigned int flexnproc_verbose = 0;
module_param_named(verbose, flexnproc_verbose, uint, 0644);

/* This module provides /proc/flexnproc which upon read() will return the number of 
 *  processes owned by the current UID.
 */

struct proc_dir_entry *flex_proc;

static ssize_t __attribute__ ((unused)) flex_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
  int cur_processes;
  size_t len;

  if  (off > 0)
    {
      *eof = 1;
      return 0;
    }

  cur_processes = atomic_read(&current->real_cred->user->processes);

  if ((len = snprintf(page, count, "0x%x\n", cur_processes)) >= count)
    {
      printk(KERN_INFO "### OVERSIZE\n");
      return -ENOSPC;
    }

  *eof = 1;
  return len;
}

static int flex_proc_show(struct seq_file *m, void *v)
{
  int cur_processes;

  cur_processes = atomic_read(&current->real_cred->user->processes);

  seq_printf(m, "0x%x\n", cur_processes);

  return 0;
}

static int flex_proc_open(struct inode *inode, struct file *file)
{
  return single_open(file, flex_proc_show, NULL);
}

static const struct file_operations flexnproc_entry_operations = 
  {
    .owner	= THIS_MODULE,
    .open	= flex_proc_open,
    .read	= seq_read,
    .release	= single_release,
  };

static int __init drop_start(void)
{
  printk(KERN_INFO "#########################################################################################\n");
  printk(KERN_INFO "###############################           COUCOU          ###############################\n");
  printk(KERN_INFO "#########################################################################################\n");
  printk(KERN_INFO "Loading flexnproc module... All your nproc are belong to us.\n");
  printk(KERN_INFO "Hello world. I will happily tell you how many processes your UID owns when you ask me.\n");

  if (!(flex_proc = proc_create(FLEX_PROC_ENTRY, 0444, NULL, &flexnproc_entry_operations)))
    {
      printk(KERN_INFO "flexnproc proc entry could not be created\n");
      return 1;
    }

  return 0;
}


static void __exit drop_end(void)
{
  if (flex_proc)
    {
      remove_proc_entry(FLEX_PROC_ENTRY, NULL);
      flex_proc = NULL;
    }
  printk(KERN_INFO "Goodbye Mr. FlexNProc\n\n");
}

module_init(drop_start);
module_exit(drop_end);

#define VERSION "0.1"

MODULE_AUTHOR("Eric Gouyer <folays@folays.net>");
MODULE_DESCRIPTION("Flexible Memory Drop Cache ver " VERSION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");
/* MODULE_LICENSE("Proprietary"); */
