#!/bin/sh
export CROSS_COMPILE=arm-none-linux-gnueabi-
make distclean
make elite1000_evm_nand_config
make HDMIIN=0 -j4
./S3VCD /vcd=TEMPLATE.VCD /map=u-boot.map /Save=u-boot.vcd /ROM=u-boot.bin
