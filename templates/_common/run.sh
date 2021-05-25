#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/<PNAME> 
else
    ./build/debug/bin/<PNAME> 
fi