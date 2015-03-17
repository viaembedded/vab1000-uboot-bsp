#!/bin/sh
export CROSS_COMPILE=arm-none-linux-gnueabi-
make distclean
make elite2000_evm_fpga_config
make -j4
