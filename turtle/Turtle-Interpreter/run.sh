#!/bin/sh

declare -a TEST=(
    "00-readme.t"
    "01-build-scanner.t"
    "02-run-scanner.t"
    "03-build-parser.t"
    "04-run-parser.t"
);

declare -a PROG=(
    "spiral"
    "gasket" 
    "if" 
    "polygon"
    "ring"
    "funky"
    "star"
    "maze1"
);

make

for i in ${TEST[@]}; do
    ./t/$i
done

for i in ${PROG[@]}; do
    ./turtle examples/$i.turtle | ./turtle.pl > $i.pgm
    ./turtle examples/$i.turtle | ./turtle.pl | convert - $i-turtle.png
    open $i-turtle.png
done

sleep 10
rm -rf *.pgm *.png
make clean

exit 0