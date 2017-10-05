obj-m += postgresql_sequence_driver.o postgresql_sequence_handler.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o ioctl ioctl.c
	gcc -o ioctl_set ioctl_set.c
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ioctl
	rm -f ioctl_set

