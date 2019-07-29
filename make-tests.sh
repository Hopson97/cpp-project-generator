sh build.sh

test_template() {
    ./bin/debug/new $1 test
    echo "\n\n==========================================="
    echo "Testing the $1 template build"
    echo "-----------------------------------"
    cd $1/test
    sh scripts/build.sh 
    echo "===========================================\n\n"
    cd ../..
}

test_template terminal
test_template sfml

rm -rf -d terminal
rm -rf -d sfml
