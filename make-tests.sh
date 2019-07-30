sh build.sh

rm -rf -d terminal
rm -rf -d sfml
rm -rf -d opengl-fps

test_template() {
    ./bin/debug/new $1 testproj
    echo "\n\n==========================================="
    echo "Testing the $1 template build"
    echo "-----------------------------------"
    cd $1/testproj
    sh scripts/build.sh
    echo "===========================================\n\n"
    cd ../..
}

test_template terminal
test_template sfml
test_template opengl-fps

if [ "$1" = "delete" ]
then
    rm -rf -d terminal
    rm -rf -d sfml
    rm -rf -d opengl-fps
fi