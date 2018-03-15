#!/bin/bash
cd ../
make 
cd Testing\ testL/
./testL > OUTPUT
echo "Finished testing"
