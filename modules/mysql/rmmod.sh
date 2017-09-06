#!/bin/bash

/etc/init.d/mysql stop
rmmod mysql_sequence_handler.ko
rmmod mysql_sequence_driver.ko
rm /dev/mysql_seq_dev
rm /dev/mysql_seq_handler
/etc/init.d/mysql start
make clean