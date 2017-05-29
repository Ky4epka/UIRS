#/bin/bash

gcc -Wall -c main.c
gcc -Wall -fPIC -c sec_priv_config_kit.c sec_priv_types.c sec_priv_look.c
gcc -shared -fPIC -o libsec_priv_look.so sec_priv_config_kit.o sec_priv_look.o sec_priv_types.o -ldl
gcc -Wall -fPIC -c sec_priv_types.c sec_priv_locallooklib.c
gcc -shared -fPIC -o libsec_priv_locallook/libsec_priv_locallook.so sec_priv_locallooklib.o sec_priv_types.o -ldl -lcap
gcc main.o -ldl -o testload.run -L. -lsec_priv_look -Wl,-rpath,. -lcap -lpam
./testload.run
cp libsec_priv_look.so ./PAM/libsec_priv_look.so
rm *.o
