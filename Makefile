lib_vlb.so: lib_vlb.c
	gcc -Wall -o lib_vlb.so --shared lib_vlb.c

gui: gui.c
	gcc -Wall -o gui `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs cairo` gui.c
