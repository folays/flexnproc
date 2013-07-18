#!/bin/sh
set -ex

cc -o libpreload-setuid-check-nproc.so -shared -fPIC preload-setuid-check-nproc.c -ldl
