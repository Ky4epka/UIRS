#/bin/bash

gcc -Wall -fPIC -c sec_priv_caps_auth.c
gcc -shared -o sec_priv_caps_auth.so sec_priv_caps_auth.o -ldl sec_priv_look.so
