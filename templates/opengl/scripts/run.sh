#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/gltexttest 
else
    ./bin/debug/gltexttest 
fi