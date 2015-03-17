#!/bin/sh
sed -i 's/SZ_2G/SZ_1G/g' include/configs/elite1000_evm.h
sed -i 's/0x80000000 - 0x800000/0x40000000 - 0x800000/g' drivers/video/cbios_interface.c
export CROSS_COMPILE=arm-none-linux-gnueabi-
make distclean
make elite1000_evm_emmc_config
make -j4
./S3VCD /vcd=TEMPLATE.VCD /map=u-boot.map /Save=u-boot.vcd /ROM=u-boot.bin
