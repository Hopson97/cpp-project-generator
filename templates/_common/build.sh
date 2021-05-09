#!/bin/bash

target_release() {
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in bin/release/"
    cd ../..
}

target_debug() {
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo "Built target in bin/debug/"
    cd ../..
}

# Create folder for distribution
if [ "$1" = "release" ]
then
    if [ -d "$<PNAME> " ]
    then
        rm -rf -d <PNAME> 
    fi

    mkdir -p <PNAME> 
fi

# Creates the folder for the binaries
mkdir -p <PNAME>
mkdir -p <PNAME>/Data
mkdir -p bin
mkdir -p bin/release
mkdir -p bin/debug
cd bin

# Builds target
if [ "$1" = "release" ]
then
    target_release
    cp bin/release/<PNAME>  <PNAME>/<PNAME> 
else
    target_debug
fi

cp -R Data <PNAME>/Data
