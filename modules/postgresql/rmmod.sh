#!/bin/bash
/etc/init.d/postgresql stop
rmmod postgresql_sequence_handler.ko
rmmod postgresql_sequence_driver.ko
rm /dev/postgresql_seq_dev
rm /dev/postgresql_seq_handler
/etc/init.d/postgresql start
make clean
