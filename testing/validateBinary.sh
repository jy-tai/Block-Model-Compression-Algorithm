#!/bin/bash

HELP_MSG="use -f <file> to test a specific binary"

#GET ABSOLUTE PATH
SCRIPT=$(readlink -f "$0")
PWD=$(dirname "$SCRIPT")

FILE=""

while getopts "f:h" opt; do
case ${opt} in 
    f) 
        FILE=$OPTARG
    ;;
    h)
        echo $HELP_MSG
    ;;
    \? )
        echo "Invalid option" && echo "$HELP_MSG"
    ;;
    *)
        echo "Need to use -f parameter"
    ;;
esac
done

# check if file input 
[ -z "$FILE" ] && echo "Need to input a file using -f <filename> -- exiting" && exit 1
echo "$FILE"

echo "compiling test runner"
g++ "$PWD/testing/harness/dcmpn.cpp" -o "$PWD/testing/harness/dcmpn"
echo "compiled test runner"

echo
echo


mkdir -p "$PWD/testOut"

for i in $(ls ./testing/cases/); do
    # remove old folder and make new one
    TESTNAME=${i%".dat"}
    FOLDERPATH=$PWD/testOut/$TESTNAME
    rm -rf $FOLDERPATH

    #declare intent to run test
    # echo "RUNNING TEST: $TESTNAME"
    mkdir -p $FOLDERPATH

    #transform windows txt files to linux test files
    cat ./testing/cases/$i | tr -d '\15\32' > "$FOLDERPATH/testCase.tmp"


    # run test binary
    if ! $FILE < "$FOLDERPATH/testCase.tmp" > "$FOLDERPATH/compressed.tmp" 2> $FOLDERPATH/bin.err ; then
        echo "[X] $TESTNAME program error, check $FOLDERPATH/bin.err"
        continue 
    fi


    # run validator
    if ! $PWD/testing/harness/dcmpn "$FOLDERPATH/testCase.tmp" "$FOLDERPATH/compressed.tmp" > $FOLDERPATH/decompressed.tmp 2> $FOLDERPATH/validator.err ; then
        echo "[X] $TESTNAME validator error, check $FOLDERPATH/validator.err"
    fi


    diff "$FOLDERPATH/decompressed.tmp" "$FOLDERPATH/testCase.tmp" > "$FOLDERPATH/diff.tmp"

    if ! diff -q "$FOLDERPATH/decompressed.tmp" "$FOLDERPATH/testCase.tmp" &>/dev/null; then
        echo "[X] $TESTNAME different decompressed result check $FOLDERPATH/diff.tmp"
    else
        echo "[✓] $TESTNAME Valid"
        rm -rf $FOLDERPATH
    fi 

    #if no diff remove FOLDERPATH and declare valid
    #else keep FOLDERPATH and declare invalid

    # rm -rf $FOLDERPATH

done
