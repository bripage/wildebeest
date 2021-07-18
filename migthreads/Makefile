#CC=/tools/emu/pathfinder-sw/21.06/bin/emu-cc
CC=/tools/emu/chick-sw/emu-20.01/bin/emu-cc
CFILES=$(wildcard *.c)

all:
	$(CC) -O3 -o wildebeest.mwx $(CFILES)

clean:
	rm -f wildebeest.mwx
	rm -f wildebeest.cdc
	rm -f wildebeest.hdd
	rm -f wildebeest.vsf
