CC=g++
UTILS_PATH=utils/

all: shuffle lib2csv csv2bin

shuffle:
	$(CC) -O3 -o shuffle shuffle.cpp

lib2csv:
	$(CC) -O3 -o libsvm2csv libsvm2csv.cpp

csv2bin:
	$(CC) -O3 -o csv2bin csv2bin.cpp


clean:
	rm -f shuffle
	rm -f lib2csv
	rm -f csv2bin