#/bin/bash

gcc -Wall -c main.c
gcc -Wall -fPIC -c securetypes.c securepriv_look.c
gcc -shared -o securepriv_look.so securepriv_look.o securetypes.o -ldl
gcc -Wall -fPIC -c securetypes.c seclocallooklib.c
gcc -shared -o seclocallooklib.so seclocallooklib.o securetypes.o -ldl
gcc main.o -ldl -o testload.so -L. -Wl,-rpath,.
./testload.so
