# !/bin/bash

make

for file in /h/ajain08/comp40/hw6/um/*.um
do
    ./um "$file" >> testing.txt 
    um "$file" >> output.txt
    diff testing.txt output.txt
    rm testing.txt
    rm output.txt
    echo $?
done