#!/bin/bash

N=500
RESOLUCIONES=(10)
THREADS=(1 2 3 4 5 6 7)
TIMEOUT=7 #segundos
REPETICIONES=(1 2 3)

# files
PHYSICS=physics
INIT=inits

# rm ./metrics/*.txt

for res in ${RESOLUCIONES[@]}
do
    
    if (( $res >= 50 )); then
	TIMEOUT=35
    elif (( $res >= 100 )); then
	TIMEOUT=420
    fi
    
    
    for rep in ${REPETICIONES[@]}
    do
	if (( $rep > 1 )); then
	    rm ./metrics/$PHYSICS.txt
	fi
	
	for thr in ${THREADS[@]}
	do
	    echo "res: $res, rep: $rep, thr: $thr"
	    export OMP_NUM_THREADS=$thr ;timeout $TIMEOUT ./Dispersion_social.out $N $res >/dev/null
         
	done
	
    done
    mv "./metrics/$PHYSICS.txt" "./metrics/$PHYSICS-$res.txt"
    mv "./metrics/$INIT.txt" "./metrics/$INIT-$res.txt"
done
