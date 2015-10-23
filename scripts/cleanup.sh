#!/bin/sh

find . -name CMakeCache.txt -o -name cmake_install.cmake -o -name Makefile -o -name CMakeFiles | xargs -n 1 -- rm -rf
rm -f tests/tmp/*
rm -f /var/tmp/slb/*
