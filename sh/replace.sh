#!/bin/bash

INPUT=$1
OUTPUT=$2

ag -l $input | while read i; do sed -i "s#$INPUT#$OUTPUT#g" $i; done

exit 0
