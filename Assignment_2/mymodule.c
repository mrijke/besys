#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/tty.h>
#include <linux/smp_lock.h>
#include <linux/syscalls.h>

void ** syscall_table;

unsigned long **find_sys_call_table(void) {
        unsigned long **sctable;
        unsigned long ptr;
//        extern int loops_per_jiffy;
        sctable = NULL;
        for (ptr = (unsigned long)&unlock_kernel; ptr < (unsigned long)&loops_per_jiffy; ptr += sizeof(void *)) {
                unsigned long *p; p = (unsigned long *)ptr;
                if (p[__NR_close] == (unsigned long) sys_close) {
                        sctable = (unsigned long **)p;
                        return &sctable[0];
                }
        }
        return NULL;
}

int start() {
        syscall_table = find_sys_call_table();
	if (syscall_table != NULL) {
		printk("I guess i got some kind of table");
	}
        return 0;
}

void stop() {
        printk("Stopping this shizzle");
}

MODULE_LICENSE("Dual BSD/GPL");

module_init(start);
module_exit(stop);


