//
// Created by Brian Page on 2020-04-22.
//
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>

int main(int argc, char **argv) {
	std::string fileName(argv[1]);
	std::string binFileName(argv[2]);

	long sample, feature, long_value;
	double value;
	//long* buffer;
	std::vector<long> buffer;

	// Read in sparse matrix saved in Matrix Market Format
	std::ifstream infile(fileName.c_str());
	if (!infile) {
		std::cout << "FAILED TO OPEN FILE!" << std::endl;
		exit(1);
	}

	std::cout << "feature file opened" << std::endl;
	std::string line;
	int i = 0, j = 0;
	bool classfile = false;
	while (std::getline(infile, line)) {
		size_t pos = 0;
		std::string token;
		i = 0;
		//std::cout << "0" << std::endl;
		pos = line.find(',');
		if (pos != std::string::npos) {
			token = line.substr(0, pos);
			line.erase(0, pos + 1);
			sample = ::atol(token.c_str());

			pos = line.find(',');
			if (pos != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + 1);
				feature = ::atol(token.c_str());
				value = ::atof(line.c_str());
			} else {
				token = line.substr(0, pos);
				line.erase(0, pos + 1);
				feature = ::atol(line.c_str());
				classfile = true;
			}
		}

		if (classfile) {
			buffer.push_back(sample);
			buffer.push_back(feature);
		} else {
			value = value * 16777216;
			long_value = (long) value;

			buffer.push_back(sample);
			buffer.push_back(feature);
			buffer.push_back(long_value);
		}
	}
	infile.close();

	std::string outFile = binFileName;
	//std::ofstream fout(outFile.c_str(), std::ios::out | std::ios::binary);
	FILE *fout;
	fout = fopen(binFileName.c_str(), "ab");

	for (i = 0; i < buffer.size(); i++) {
		fwrite(&buffer[i], sizeof(long), 1, fout);
	}
	fclose(fout);

	std::ifstream infile2(binFileName.c_str(), std::ios::in | std::ios::binary);
	if (!infile2) {
		std::cout << "FAILED TO OPEN FILE!" << std::endl;
		exit(1);
	}

	long temp1, temp2, temp3;
	infile2.read((char *) &temp1, sizeof(long));
	infile2.read((char *) &temp2, sizeof(long));
	infile2.read((char *) &temp3, sizeof(long));
	printf("%ld,%ld,%ld\n", temp1, temp2, temp3);

	infile2.close();

	//	free(buffer);
	//}
	std::cout << "Done!" << std::endl;

	return 0;
}