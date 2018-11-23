#!/bin/bash
args=("$@")
echo args[0] is ${args[0]} #${args[1]} ${args[2]}
NUMBER=0
TESTS="./test/*.bin"

for T in $TESTS
do
    fullfilename="${T%.*}" #get filename without extension
    filename=$(basename "$fullfilename") #get rid of path to the file

    testname="${filename%%.*}" #get testname which is before first "."
    author="${filename#*.}" #get what is after the first "."
    author="${author%%.*}" #get author which is between first and second "."
    result="${filename##*.}" #get expected result which is between last and second last dots
#echo $result
if [[ $testname == *"termination"* ]];
then
    cat ./test/input.txt | ${args[0]} $T
    EXITCODE=$?
else
    ${args[0]} $T
    EXITCODE=$?
fi



#needs to get result from simulator to replace 0 here!!!!!!
if [ $result -eq $EXITCODE ]; #space between square brackets are necessary
then
    echo "$NUMBER, $testname, pass, $author" #author is the between first and second dots
else
    echo "$NUMBER, $testname, fail, $author (expected result $result, got $EXITCODE)"
fi
    NUMBER=$((NUMBER+1))
done
