#!/bin/bash

mkdir test_data
cd test_data
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/a8a -o a8a_train.dat
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/a8a.t a8a_test.dat

../utilities/shuffle -i a8a_train.dat > a8a_train.shuf
../utilities/libsvm2csv -i a8a_train.shuf -s 22696 > a8a_train.csv
../utilities/libsvm2csv -i a8a_test.dat -s 9865 > a8a_test.csv
../utilities/csv2bin -i a8a_train.csv -o a8a_train.bin
../utilities/csv2bin -i a8a_test.csv -o a8a_test.bin
