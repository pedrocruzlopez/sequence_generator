#!/bin/bash
cd build/
cmake -DCMAKE_INSTALL_PREFIX=/usr ../
make 
make install
systemctl enable seqgen.service
systemctl enable seqgen_backup.service
