#!/bin/bash

cd metrics

python3 parallel_efficiency.py inits-50.txt inits-100.txt init_efficiency_final.pdf
python3 parallel_efficiency.py physics-50.txt physics-100.txt physics_efficiency_final.pdf
python3 speedup.py physics-50.txt physics-100.txt physics_speedup_final.pdf
python3 speedup.py inits-50.txt inits-100.txt init_speedup_final.pdf

