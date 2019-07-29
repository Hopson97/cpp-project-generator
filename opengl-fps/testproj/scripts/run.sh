#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/testproj 
else
    ./bin/debug/testproj 
fi