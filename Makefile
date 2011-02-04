lib_vlb.so: lib_vlb.c
	gcc -Wall -o lib_vlb.so --shared lib_vlb.c
