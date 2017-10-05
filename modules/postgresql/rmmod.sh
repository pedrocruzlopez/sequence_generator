#!/bin/bash

rmmod postgresql_sequence_handler.ko
rmmod postgresql_sequence_driver.ko
rm /dev/postgresql_seq_dev
rm /dev/postgresql_seq_handler
make clean