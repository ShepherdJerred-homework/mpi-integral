#!/usr/bin/env bash
file="integral"

mpicc src/${file}.c -o out/${file}
mpirun -np 64 -hostfile ~/hostfile out/${file} < tests/test_1_input.txt > tests/test_1_output.txt
diff tests/test_1_output.txt tests/test_1_expected_output.txt

#mpirun -np 4 -hostfile ~/hostfile out/${file}
#mpirun -np 2 out/${file}
