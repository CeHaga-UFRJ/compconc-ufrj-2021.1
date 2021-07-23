#!/bin/bash
for i in {1..5}
do
  for dim in 500 1000 2000
  do
    for nthreads in 1 2 4
    do
      echo "Running ${i}_${dim}_${nthreads}";
      ./a.out ${dim} ${nthreads} > out/out_${i}_${dim}_${nthreads}.txt;
    done
  done
done