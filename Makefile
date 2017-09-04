obj-m += mysql_sequence_driver.o
MYSQL_CFLAGS = `mysql_config --cflags`
MYSQL_PLUGINDIR=`mysql_config --plugindir`

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o ioctl ioctl.c
	gcc $(MYSQL_CFLAGS) -shared -fPIC -o mysql_get_sequence.so mysql_get_sequence.c
	cp mysql_get_sequence.so $(MYSQL_PLUGINDIR)
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ioctl
	rm -f mysql_get_sequence.so
	rm -f mysql_get_uuid.so
