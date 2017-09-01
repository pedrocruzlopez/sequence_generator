obj-m := sequence_driver.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
MYSQL_CFLAGS = `mysql_config --cflags`
MYSQL_PLUGINDIR=`mysql_config --plugindir`


default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o ioctl ioctl.c
	gcc $(MYSQL_CFLAGS) -shared -fPIC -o get_sequence.so get_sequence.c
	cp get_sequence.so $(MYSQL_PLUGINDIR)
	
	
clean:
	rm -f ioctl
	rm -f get_sequence.so
	rm -f get_uuid.so
	$(MAKE) -C $(KDIR) M=$(PWD) clean
