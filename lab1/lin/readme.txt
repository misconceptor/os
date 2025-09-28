g++ -fPIC -c num/Number.cpp -o Number.o
ar rcs num/libnumber.a Number.o
g++ -fPIC -shared vec/Vector.cpp num/libnumber.a -o vec/libvector.so
g++ main.cpp -Lnum -lnumber -Lvec -lvector -o main
export LD_LIBRARY_PATH=vec:$LD_LIBRARY_PATH
./main