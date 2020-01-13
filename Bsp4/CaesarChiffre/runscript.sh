
cd ./Kernel/
make clean
make
sudo ./unload.sh
sudo ./load.sh
konsole --noclose -e "cat /dev/caesare" &

cd ..
cd ./Testing
make
make run

