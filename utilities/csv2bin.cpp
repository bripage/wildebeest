/*
 * CSV2BIN: This utility program will read in a 3 column csv file and output its contents to binary.
 *          Note: csv data is expected to be numeric in every column, and binary output uses the 64 bit integer
 *          (int64_t) datatype.
 *
 * Author: Brian Page
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include "stdint.h"

int main(int argc, char **argv) {
	std::string argTemp;
	std::string fileName;
	std::string binFileName;

	for (int i = 1; i < argc; i= i+2){
		argTemp = argv[i];
		if (argTemp == "-i"){
			// load data file.
			fileName = argv[i+1];
		} else if (argTemp == "-o") {
			// save converted data file.
			binFileName = argv[i + 1];
		} else if (argTemp == "--help"){
            printf("CSV2BIN: This utility program will read in a 3 column csv file and output its contents to"
                   "binary. Note: csv data is expected to be numeric in every column, and binary output uses the 64 bit"
                   "integer (int64_t) datatype.\n\n");
			printf("Usage: ./csv2bin [OPTION] <argument> ...\n");
            printf("Options:\n");
            printf("-i <file> \tInput filename (assumes csv structure is: sample_id,feature_id,feature_value)\n");
            printf("-o <file> \tOutput filename\n");
			exit(0);
		} else {
            printf("%s Is not a valid parameter. Try --help for more information.\n EXITING!\n", argv[i]);
			exit(0);
		}
	}

    int64_t long_value;
	long sample, current_sample = 0, feature, classification;
	double value;
	std::vector<int64_t> buffer;

	// Read in sparse matrix saved in Matrix Market Format
	std::ifstream infile(fileName.c_str());
	if (!infile) {
		std::cout << "FAILED TO OPEN FILE!" << std::endl;
		exit(1);
	}

	std::cout << "feature file opened" << std::endl;
	std::string line;
	long i = 0, j = 0;
	while (std::getline(infile, line)) {
		size_t pos = 0;
		std::string token;

		pos = line.find(',');
		token = line.substr(0, pos);
		line.erase(0, pos + 1);
		sample = ::atol(token.c_str());

		pos = line.find(',');
		token = line.substr(0, pos);
		line.erase(0, pos + 1);
		feature = ::atol(token.c_str());

		pos = line.find(',');
		token = line.substr(0, pos);
		line.erase(0, pos + 1);
		value = ::atof(token.c_str());
        classification = ::atol(line.c_str());

        if (current_sample != sample) {
            current_sample = sample;
        }
		value = value * 16777216;
		long_value = (int64_t) value;

		// add to buffer
		buffer.push_back((int64_t) sample);
		buffer.push_back((int64_t) feature);
		buffer.push_back((int64_t) long_value);
        buffer.push_back((int64_t) classification);
	}
	infile.close();

	FILE *fout;
	fout = fopen(binFileName.c_str(), "ab");

	for (i = 0; i < buffer.size(); i++) {
		fwrite(&buffer[i], sizeof(int64_t), 1, fout);
	}
	fclose(fout);

	std::cout << "Done!" << std::endl;
	return 0;
}