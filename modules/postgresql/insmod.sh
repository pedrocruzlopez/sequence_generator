#!/bin/bash
echo "Installing modules..."
insmod postgresql_sequence_driver.ko
insmod postgresql_sequence_handler.ko
echo "Creating devices files..."


if [ -e /dev/postgresql_seq_dev ]
then
    echo "devices already created"
else
   	mknod /dev/postgresql_seq_dev c 200 0
	mknod /dev/postgresql_seq_handler c 201 0
fi

echo "Success!"


