#!/bin/bash
cd ../
make 
cd Testing/ 
./testL > OUTPUT
echo "Finished testing"
