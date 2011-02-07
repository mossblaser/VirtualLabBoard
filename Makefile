lib_vlb.so: lib_vlb.c gui.o
	gcc -Wall -o lib_vlb.so -pthread --shared lib_vlb.c

gui.o : gui.c
	gcc -Wall -c `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs cairo` gui.c
