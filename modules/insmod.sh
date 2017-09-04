#!/bin/bash

insmod mysql_sequence_driver.ko
mknod /dev/mysql_seq_dev c 100 0
mknod /dev/mysql_seq_handler c 101 0
insmod mysql_sequence_handler.ko
grep sequences /proc/kallsyms 
