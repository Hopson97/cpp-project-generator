sh build.sh

./bin/debug/new terminal terminal
./bin/debug/new sfml sfml

rm -rf -d test-projects
mkdir test-projects

mv terminal/terminal test-projects/terminal
mv sfml/sfml test-projects/sfml

rm -rf -d terminal
rm -rf -d sfml

#konsole --workdir test-projects/sfml
konsole --workdir test-projects/terminal