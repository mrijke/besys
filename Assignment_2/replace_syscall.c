/*
 * replace_syscall.c
 *
 *  Created on: Jan 9, 2010
 *      Author: Daniel van der Steeg (s0209090) & Maarten Rijke (s0191221)
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp_lock.h>
#include <linux/syscalls.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/path.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>

//Method declarations
asmlinkage int (*real_open) (const char* pathname, int flags, int mode);
asmlinkage int our_open(const char *pathname, int flags,int mode);
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

//Global variable declarations
void **syscall_table;
struct proc_dir_entry *labosProc;
int numberOfOpenedFiles;

/* Searches through a certain memory region to find the syscall table.
 */
unsigned long **find_sys_call_table(void)
{

    unsigned long **sctable;
    unsigned long ptr;
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
    return NULL;
}

/* Method run when the module is loaded. 
 * It readies everything to be able to intercept the sys_open(...) system call
 */
int start(void)
{
    syscall_table = (void*)find_sys_call_table();
    if(syscall_table == NULL){
        printk("[OSLAB] Fatal Error: could not find system call table!\n");
        return 0;
    }

    real_open = syscall_table[__NR_open];
    syscall_table[__NR_open] = (unsigned long *)our_open;

    numberOfOpenedFiles = 0;
    //create_proc_read_entry is possible as well.
    labosProc = create_proc_entry("labos", S_IFREG | S_IRUGO, NULL);

    //if it failed, remove it, and give an error message
    if (labosProc == NULL) {
        remove_proc_entry("labos", NULL);
        printk(KERN_ALERT "Error: Could not initialize '/proc/labos'\n");
    }else{
        //These are some options needed for the proc entry to work
        labosProc->read_proc = procfile_read;        			 //Link to the method that is called when /proc/labos is opened
        labosProc->uid       = 0;                          //userid that is the owner of the entry
        labosProc->gid       = 0;                          //groupid
    }
    printk("[OSLAB] Loaded Kernel Module\n");
    return 0;
}

/* Interception method
 * This method is called instead of the sys_open() system call.
 * After logging, it redirects the call to sys_open()
 */
asmlinkage int our_open(const char *pathname, int flags,int mode)
{
    int r,error;
    char *fullpath,*prevfullpath,*actionText;
    struct dentry *cur,*prev;
    fullpath = prevfullpath = NULL;
    error = 0;

    //Checking flags
    if(flags & O_RDWR)
        actionText = "reading & writing in";
    else if(flags & O_WRONLY)
        actionText = "writing to";
    else if((flags & O_ACCMODE) == 0)
        actionText = "reading from";
    else
        actionText = "working in";

    //This only works with absolute paths
    if(strncmp(pathname, "/tmp/",5) == 0){
        printk("[OSLAB] '%s' is %s '%s'!\n",current->comm,actionText,pathname);
        numberOfOpenedFiles++;
		}
    //if it is not absolute, it shouldn't start with a '/'
    //so here come all the relative paths
    //this check is mainly needed because 1 process might be opening only 1 file from /tmp/ 
    //with 'tmp' as the PWD, but all files it opens are logged. This happens because the program
    //uses absolute paths for the other files.
    else if(strncmp(pathname, "/",1) != 0)
    {
			 //get the working directory of the current task
			 prev = cur = current->fs->pwd.dentry;
			 //while we are not at the root
			 while(strncmp(cur->d_name.name,"/",1) != 0){
			 	  //get the parent
					prev = cur;
					cur = cur->d_parent;
			 }

			 //if the directory one from the root (the one that is traversed before we reach the root)
			 //is the 'tmp' folder
			 if(strncmp(prev->d_name.name,"tmp",3) == 0) {
					 //reset these vars to the current working directory
					 prev = cur = current->fs->pwd.dentry;
					 fullpath = kmalloc(sizeof(char)*(strlen(pathname)+1),GFP_KERNEL);
					 if(fullpath == NULL)
					  	error = 1;
					 else {
					  	strcpy(fullpath,"/");
					   	strcat(fullpath,pathname);
					 }
					 //same loop, different content. Doing this for every 'open'-syscall would
					 //make the system quite a lot slower + RAM usage would increase drastically
					 while(strncmp(cur->d_name.name,"/",1) != 0 && error != 1){
							prevfullpath = kmalloc(sizeof(char)*strlen(fullpath),GFP_KERNEL);
							if(prevfullpath == NULL){
								error = 1;
								break;
							};
							strcpy(prevfullpath,fullpath);
							kfree(fullpath);
							fullpath = kmalloc(sizeof(char)*(1+strlen(cur->d_name.name)+strlen(prevfullpath)),GFP_KERNEL);
							if(fullpath == NULL){
									error = 1;
									break;
							}
							//add the '/' to separate the dir names
							strcpy(fullpath,"/");
							//append the dir name after the '/'
							strcat(fullpath,cur->d_name.name);
							strcat(fullpath,prevfullpath);
							kfree(prevfullpath);
							//get the parent directory
							prev = cur;
							cur = cur->d_parent;
					 }
					 if(error == 1){
					   if(fullpath != NULL)     kfree(fullpath);
					   if(prevfullpath != NULL) kfree(prevfullpath);
					   printk("[OSLAB] There was an error while allocating memory!\n");
					   printk("[OSLAB] Partial opening information:\n");
					   printk("[OSLAB] '%s' is %s %s!\n",current->comm,actionText,pathname);
					 }else{
					   //print the message
				 	   printk("[OSLAB] '%s' is %s '%s'!\n",current->comm,actionText,fullpath);
						 kfree(fullpath);
				 }
				 numberOfOpenedFiles++;
			 } // if(strncmp(prev->d_name.name,"tmp",3) == 0)

	} // else if(strncmp(pathname, "/",1) != 0)

  //open the file for real
  r = real_open(pathname, flags, mode);
  return r;
}

/* The read method for the proc-entry
 * It returns the number of files in /tmp/ that have been opened.
 */
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;

    //if offset has been given it means this isnt the first read.
    //so we return size = 0, because we don't have any other information
    if (offset > 0) {
        ret  = 0;
    } else {
        //offset = 0, so we print the message containing the requested info
        ret = sprintf(buffer, "Number of files opened in /tmp/: %i\n",numberOfOpenedFiles);
    }

    return ret;
}

/* Method run when the module is unloaded
 * It cleans everything up.
 */
void stop(void)
{
    syscall_table[__NR_open] = (unsigned long *)real_open;
    remove_proc_entry("labos", NULL);
    printk("[OSLAB] Unloaded Kernel Module\n");
}

MODULE_LICENSE("Dual BSD/GPL");

module_init(start);
module_exit(stop);
