#!/bin/sh

PORT=$1
make clean
make -f Makefile2

until sudo -E env "PATH=$PATH" make flash ESPPORT=$PORT; do
	sleep 1
done

sudo screen $PORT 115200
