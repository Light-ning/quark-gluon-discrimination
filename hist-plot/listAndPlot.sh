#!/usr/bin/bash

hadd -f $1.root $1_*.root

rm outfile
for i in $(find *$1*); do
    echo $i >> outfile
done

root -b -q "trial.C(\"$1\")"

