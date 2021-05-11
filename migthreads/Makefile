CC=/tools/emu/emu-20.01/bin/emu-cc
CFILES=$(wildcard *.c)

all:
	$(CC) -O3 -o hogwild.mwx $(CFILES)

clean:
	rm -f hogwild.mwx
	rm -f hogwild.cdc
	rm -f hogwild.hdd
	rm -f hogwild.vsf
