#!/bin/bash

/etc/init.d/mysql stop
rmmod mysql_sequence_driver.ko
rmmod mysql_sequence_handler.ko
rm /dev/mysql_seq_dev
rm /dev/mysql_seq_handler
/etc/init.d/mysql start
