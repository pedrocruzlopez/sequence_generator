#!/bin/bash

echo "Cleaning directories.."
make clean
echo "Compiling modules..."
make
echo "Installing modules..."
insmod mysql_sequence_driver.ko
insmod mysql_sequence_handler.ko
echo "Creating devices files..."
mknod /dev/mysql_seq_dev c 100 0
mknod /dev/mysql_seq_handler c 101 0
echo "Success!"


