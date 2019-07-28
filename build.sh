#!/bin/bash

mkdir -p bin
cd bin 
mkdir -p release
mkdir -p debug

if [ "$1" = "release" ]
then
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in bin/release/"
    cp new ../../../../new
    cd ../../../../
    rm -rf -d templates/
    cp -r terminal/cpp-project-generator/templates/ templates/
else
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo ""

fi
