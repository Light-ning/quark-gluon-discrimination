#!/usr/bin/bash

hadd -f $1.root $1_*.root

rm outfile
for i in $(find $1_*.root); do
    echo $i >> outfile
done

root -b -q "draw.C(\"$1.root\")"
root -b -q "drawpt.C(\"outfile\", \"$1.root\")"

