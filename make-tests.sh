sh build.sh

delete_all() {
    rm -rf -d terminal
    rm -rf -d sfml
    rm -rf -d opengl
}

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
delete_all

test_template terminal
test_template sfml
test_template opengl

if [ "$1" = "delete" ]
then
    delete_all
fi