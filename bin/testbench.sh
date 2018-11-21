#!/bin/bash
args=("$@")
echo ${args[0]} #${args[1]} ${args[2]}
NUMBER=0
TESTS="./test/*"

echo $?

for T in $TESTS
do

    echo $T
    ${args[0]} $T
    EXITCODE=$?
    filename=$(basename "$T") #get rid of path to the file
    filename="${filename%.*}" #get filename without extension
    testname="${filename%%.*}" #get testname which is before first "."
    author="${filename#*.}" #get what is after the first "."
    author="${author%%.*}" #get author which is between first and second "."
    #echo $filename
    result="${filename##*.}" #get expected result which is between last and second last dots


#needs to get result from simulator to replace 0 here!!!!!!
if [ $result -eq $EXITCODE ]; #space between square brackets are necessary
then
    echo $NUMBER, $testname, pass, $author #author is the between first and second dots
else
    echo $NUMBER, $testname, fail, $author "(expected result $result, got $EXITCODE)"
fi
    NUMBER=$((NUMBER+1))
done
