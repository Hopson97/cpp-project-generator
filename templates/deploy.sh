sh scripts/build.sh release 


mkdir <PNAME>
mkdir <PNAME>/res

cp bin/release/<PNAME> <PNAME>
cp -r -d res <PNAME>/res

echo "Deploy build created."
echo "cd <PNAME> to find it"