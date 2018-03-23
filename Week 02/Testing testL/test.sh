#!/bin/bash
echo "Removing old output"
rm -f OUTPUT
cd ../
make
cd Testing\ testL/
./testL > OUTPUT
echo "Finished testing"
