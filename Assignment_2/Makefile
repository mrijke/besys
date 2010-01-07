obj-m += trivial_module.o
obj-m += read_syscalltable.o
obj-m += procinfo.o
obj-m += replace_syscall.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
