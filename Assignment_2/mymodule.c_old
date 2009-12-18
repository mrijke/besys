#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/tty.h>

MODULE_LICENSE("Dual BSD/GPL");

int start(void) 
{
	printk(KERN_INFO "blaat: Superawesome LKM.\n");
	console_print("wat bennegedanallemaalaanhetdoendan.");
	return 0;
}

void stop(void)
{
	printk(KERN_ALERT "blaat: Your system just got less awesome because you rmmodded an awesome kernelmodz\n");
	console_print("Later");
}

module_init(start);
module_exit(stop);
