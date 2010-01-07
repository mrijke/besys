/*
 ============================================================================
 Name        : assignment2.c
 Author      : Sjikke
 Version     :
 Copyright   : I own everything
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <asm/current.h>
//#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/unistd.h>
#include <linux/smp_lock.h>
#include <linux/syscalls.h>
#include <linux/string.h>

int (*real_open) (const char* pathname, int flags, int mode);
void **syscall_table;

unsigned long **find_sys_call_table(void)
{

        unsigned long **sctable;
        unsigned long ptr;
        //extern int loops_per_jiffy;
        sctable = NULL;
        for (ptr = (unsigned long)&unlock_kernel; ptr < (unsigned long)&loops_per_jiffy; ptr += sizeof(void *))
        {
                unsigned long *p; p = (unsigned long *)ptr;
                if (p[__NR_close] == (unsigned long) sys_close)
                {
                        sctable = (unsigned long **)p;
                        return &sctable[0];
                }
        }
        printk("blaat, returning null");
        return NULL;
}

int our_open(const char *pathname, int flags,int mode);

int start(void)
{
   syscall_table = (void*)find_sys_call_table();
   if(syscall_table == NULL){
      printk("[OSLAB] Failure with hacking syscall table\n");
      return 0;
   }
//   printk(KERN_INFO "The user space process is '%s'\n", current->comm);
//   printk(KERN_INFO "The user space PID is  %i\n", current->pid);
   real_open = (void*)syscall_table[__NR_open];
   real_open = (int(*)(const char *pathname, int flags, int mode))(syscall_table[__NR_open]);
   syscall_table[__NR_open] = (unsigned long)our_open;
   printk("Succesfully loaded Assignement2 logger thingy\n");
   return 0;
}

int our_open(const char *pathname, int flags,int mode)
{
		//if(strlen(pathname) >= 5) {
		//	if(strncmp(pathname,"/tmp/",5) == 0) {
		//		printk("Application %s.", current->comm);
		//	}
		//}
        int r;
        r = real_open(pathname, flags, mode);
        return r;
}
void stop(void)
{
        printk("[OSLAB]Stopping H4cKS3rV!c3 Pr0 2.4" );
        syscall_table[__NR_open] = (unsigned long)real_open;
}

MODULE_LICENSE("Dual BSD/GPL");

module_init(start);
module_exit(stop);
