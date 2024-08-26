#!/bin/bash

for i in $(ls ../caseDefinitions/); do
    if echo "$i" | grep -q ".test"; then
        echo "skipping .test"
    else
        cat ../caseDefinitions/$i | ./bin/out "../cases/$(echo $i | sed "s/\.case\b/.dat/g")"
    fi
done
