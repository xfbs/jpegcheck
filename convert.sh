#!/bin/bash
set -e

git submodule init -q
git submodule update -q

bash -c "make -j 4 -s" &
bash -c "cd ijg && test -f Makefile || ./configure -q && make -j 4 > /dev/null" &

convert "$1" "$1.ppm"

wait < <(jobs -p)

ijg/cjpeg -outfile "$1.jpg" "$1.ppm"

./jpegcheck "$1.jpg"
