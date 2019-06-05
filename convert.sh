#!/bin/bash

git submodule init -q
git submodule update -q

bash -c "make -s" &
bash -c "cd ijg && test -f Makefile || ./configure -q && make > /dev/null" &

convert "$1" "$1.ppm"

wait < <(jobs -p)

ijg/cjpeg -outfile "$1.jpg" "$1.ppm"

./jpegcheck "$1.jpg"
