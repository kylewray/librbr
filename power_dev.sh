#!/bin/bash

rm *.o
rm -rf obj
rm -rf tests/obj
rm -rf tests/tmp
rm Makefile
rm perform_tests

python makemake.py
make

./perform_tests

