COMMON_FLAGS=-Wall

lib_vlb.so: lib_vlb.c
	gcc $(COMMON_FLAGS) -o lib_vlb.so --shared lib_vlb.c

gui : gui.c
	gcc $(COMMON_FLAGS) -o gui `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs cairo` gui.c

.PHONEY : clean
clean :
	-rm gui lib_vlb.so
