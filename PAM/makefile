all: build

build: main.o sec_priv_caps_auth.o sec_priv_caps_auth.so
	gcc main.o -ldl -o testload.run sec_priv_caps_auth.so sec_priv_look.so sec_priv_caps_auth.o -L. -Wl,-rpath,. -lcap -lpam

main.o: main.c
	gcc -c main.c

sec_priv_caps_auth.so: sec_priv_caps_auth.o
	gcc -shared -o sec_priv_caps_auth.so sec_priv_caps_auth.o -ldl -lcap sec_priv_look.so -lpam

sec_priv_caps_auth.o: sec_priv_caps_auth.c
	gcc -c sec_priv_caps_auth.c

clean:
	rm -rf *.o build
