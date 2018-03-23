#!/bin/bash
echo "Removing old test files"
rm -f TEXT OUTPUT_EXPECTED OUTPUT_SELF DIFFERENCE.txt
echo "Creating test files"
seq 100 > TEXT
echo "Starting tests"
./myed TEXT < COMMANDS > OUTPUT_SELF
./myed_correct TEXT < COMMANDS > OUTPUT_EXPECTED
diff OUTPUT_SELF OUTPUT_EXPECTED > DIFFERENCE.txt
echo "Finished testing"
