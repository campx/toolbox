#!/bin/bash

INPUT=$1
OUTPUT=$2
PREFIX=$3

find . -name "*$INPUT*" | while read i;
do
    TARGET=`echo $i | sed "s#$INPUT#$OUTPUT#g"` 
    SOURCE=$i
    if [ "" != "$PREFIX" ];
    then
        "$PREFIX" mv -v "$SOURCE" "$TARGET"
    else
        mv -v "$SOURCE" "$TARGET"
    fi
done

exit 0
