#!/bin/bash
./myed TEXT < COMMANDS > OUTPUT_SELF
./myed_correct TEXT < COMMANDS > OUTPUT_EXPECTED
diff OUTPUT_SELF OUTPUT_EXPECTED > DIFFERENCE.txt
echo "Finished testing"
