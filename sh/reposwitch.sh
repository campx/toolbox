#!/bin/bash

TARGET=$1

find . -name CMakeLists.txt.in | while read i; 
do
    if [ "$TARGET" == "online" ];
    then
        sed -i 's/GIT_REPOSITORY/#OFFLINE\ GIT_REPOSITORY/g' $i
        sed -i 's/#ONLINE\ #OFFLINE\ //g' $i
    elif [ "$TARGET" == "offline" ];
    then
        sed -i 's/GIT_REPOSITORY/#ONLINE\ GIT_REPOSITORY/g' $i
        sed -i 's/#OFFLINE\ #ONELINE\ //g' $i
    fi
done
