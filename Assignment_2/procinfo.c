#include <linux/module.h>       // for all modules
#include <linux/init.h>         // for entry/exit macros
#include <linux/kernel.h>       // for printk priority macros
#include <asm/current.h>        // process information, just for fun
#include <linux/sched.h>        // for "struct task_struct"

static int hi(void)
{
        printk(KERN_INFO "Hello from hi module.\n");
        printk(KERN_INFO "The user space process is '%s'\n", current->comm);
        printk(KERN_INFO "The user space PID is  %i\n", current->pid);
        return 0;       // to show a successful load
}

static void bye(void)
{
        printk(KERN_INFO "Goodbye from hi module.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("Robert P. J. Day");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("2:1.0") ;
MODULE_DESCRIPTION("You have to start somewhere.");
