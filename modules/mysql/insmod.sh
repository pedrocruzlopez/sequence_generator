#!/bin/bash
echo "Installing modules..."
insmod mysql_sequence_driver.ko
insmod mysql_sequence_handler.ko
echo "Creating devices files..."


if [ -e /dev/mysql_seq_dev ]
then
    echo "devices already created"
else
   	mknod /dev/mysql_seq_dev c 100 0
	mknod /dev/mysql_seq_handler c 101 0
fi

echo "Success!"


