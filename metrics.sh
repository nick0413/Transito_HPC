#!/bin/bash

rm ./metrics/*.txt

for j in {1..2}
do
    if (( $j > 1 )); then
    rm ./metrics/physics.txt
    fi
    echo "j $j"
    for i in {5..6}
    do  export OMP_NUM_THREADS=$i ;timeout 20  ./Dispersion_social.out >/dev/null
    done
done
