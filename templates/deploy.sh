sh scripts/build.sh release 

rm -rf -d <PNAME>

mkdir <PNAME>

cp bin/release/<PNAME> <PNAME>

echo "Deploy build created."
echo "cd <PNAME> to find it"