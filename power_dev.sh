#!/bin/bash

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/lib/:/usr/lib64:/usr/local/lib

rm *.o
rm -rf librbr/obj
rm -rf librbr_tests/obj
rm -rf librbr_tests/tmp
rm Makefile
rm librbr_tests/perform_tests

python makemake.py
make

mv perform_tests librbr_tests/perform_tests
cd librbr_tests
./perform_tests
cd ..

