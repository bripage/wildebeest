CC=icc
CFILES=$(wildcard *.c)

all:
	$(CC) -std=c99 -fcilkplus -O3 -o wildebeest_conv $(CFILES)

clean:
	rm -f wildebeest_conv
