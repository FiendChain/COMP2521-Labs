 #!/bin/bash
total_values=5000
if [ -z "$1" ]
then
    echo No arguments supplied, using default
else
    total_values=$1
fi
echo Total values: $total_values
 ./randl "$total_values" > IN_0
 time  ./usel < IN_0 > /dev/null 
 time sort < IN_0 > /dev/null
 rm IN_0
