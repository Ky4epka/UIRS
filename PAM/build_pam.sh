#/bin/bash

gcc -Wall -fPIC -c sec_priv_caps_auth.c
gcc -shared -fPIC sec_priv_caps_auth.o -o libsec_priv_caps_auth.so -Wl,-rpath,/usr/lib/_sec_priv -ldl -lcap -lpam -L/usr/lib/_sec_priv -lsec_priv_look
gcc -Wall -fPIC -c main.c
gcc main.o -ldl -o testload.run -L. -lsec_priv_caps_auth -Wl,-rpath,. -lcap -lpam
./testload.run
rm *.o
