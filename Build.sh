#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

for DIR in $parent_path/src/* ; do
    echo "Building: $DIR"
    BINDIR=${DIR/"src"/"bin"}
    mkdir -p $BINDIR
    # g++ $(echo $DIR/*.*pp) -o "$BINDIR/out.exe" 
done