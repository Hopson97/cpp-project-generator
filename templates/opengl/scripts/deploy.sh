#!/bin/bash

sh scripts/build.sh release 

rm -rf -d gltexttest

mkdir gltexttest

cp bin/release/gltexttest gltexttest
cp -r res gltexttest

echo "Deploy build created."
echo "cd gltexttest to find it"