obj-m += postgresql_sequence_driver.o postgresql_sequence_handler.o

PG_INCLUDE=`pg_config --includedir-server`
PG_LIBDIR=`pg_config --libdir`
PG_PACKAGE=`pg_config --pkglibdir`

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o ioctl ioctl.c
	gcc -o ioctl_set ioctl_set.c
	gcc -ggdb -fpic -I$(PG_INCLUDE) postgresql_get_sequence.c -c -o postgresql_get_sequence.o
	gcc -ggdb -shared -o postgresql_get_sequence.so postgresql_get_sequence.o
	cp postgresql_get_sequence.so $(PG_PACKAGE)/postgresql_get_sequence.so
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ioctl
	rm -f ioctl_set
	rm -f postgresql_get_sequence.so
	rm -f postgresql_get_sequence.o

