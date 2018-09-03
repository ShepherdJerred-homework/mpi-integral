#!/usr/bin/env bash
file="integral"

mpicc src/${file}.c -o out/${file}
#mpirun -np 4 -hostfile ~/hostfile out/${file} < blur_test_1_input.txt > blur_test_1_output.txt
#diff blur_test_1_output.txt blur_test_1_expected_output.txt

#mpirun -np 4 -hostfile ~/hostfile out/${file}
mpirun -np 2 out/${file}
