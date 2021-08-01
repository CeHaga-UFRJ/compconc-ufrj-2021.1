#!/bin/bash
for dim in 100000 10000000 1000000000
do
  for nthreads in 2 4
  do
    echo "Running ${dim}_${nthreads}";
    ./a.out ${dim} ${nthreads} > out/out_${dim}_${nthreads}.txt;
  done
done