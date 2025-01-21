#!/bin/bash

for size in 1000; do
  for type in {0}; do
    echo "Running type $type with size ${size}x${size}..."
    perf stat -e cache-references,cache-misses -o perf_${type}_${size}.txt \
      ./main $type $size $size $size "./input_path/" "./output_path/" 
    perf record -g -o perf_record_${type}_${size}.data \
      ./main $type $size $size $size "./input_path/" "./output_path/"
  done
done
