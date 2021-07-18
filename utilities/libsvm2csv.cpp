/*
 * Lib2Csv: This utility program will read in an input in LibSVM format (https://www.csie.ntu.edu.tw/~cjlin/libsvm/).
 *          Once read, training data is then output to the console in CSV format. Note: This utility will normalize
 *          training data to [0-1] if not already within this range.
 *
 * Author: Brian Page
 */

#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* strtod */
#include <unistd.h>
#include "stdint.h"

int main(int argc, char *argv[]) {
	std::string argTemp;
	std::string inputDataFile;
	long totalSamples;

	for (int i = 1; i < argc; i= i+2){
		argTemp = argv[i];
		if (argTemp == "-i"){
			// load data file.
			inputDataFile = argv[i+1];
		} else if (argTemp == "-s"){
            totalSamples = ::atoi(argv[i+1]);
		} else if (argTemp == "-help" || argTemp == "--help" || argTemp == "-h"){
		    printf("Lib2Csv: This utility program will read in an input in LibSVM format"
             "(https://www.csie.ntu.edu.tw/~cjlin/libsvm/). Once read, training data is then output to the console in"
             "CSV format. Note: This utility will normalize training data to [0-1] if not already within this range.\n\n");
		    printf("Usage: ./libsvm2csv [OPTION] <argument> ...\n");
            printf("Options:\n");
            printf("-i <file> \tIn put filename and path\n");
            printf("-s <sample_count> \tTraining data sample count\n");
            exit(0);
		} else {
            printf("%s Is not a valid parameter. Try --help for more information.\n EXITING!\n", argv[i]);
			exit(0);
		}
	}

    std::vector< std::vector<double> > vals;
    std::vector< std::vector<long> > features;
    std::vector<long> classes;

	for (long i = 0; i < totalSamples; i++){
        std::vector<double> tempSampleVec;
        vals.push_back(tempSampleVec);
        std::vector<long> tempSampleVec2;
        features.push_back(tempSampleVec2);
	}

	// Read in libsvm file
	std::ifstream infile(inputDataFile);
	if (!infile) {
		std::cout << "FAILED TO OPEN FILE!" << std::endl;
		exit(1);
	}

    long classification, feature, sample = 0;
	double value;
	std::string line;

	long i = 0;
    double max_val = 0.0;
	while (std::getline(infile, line)) {
		size_t pos = 0;
		std::string token;
		while ((pos = line.find(' ')) != std::string::npos) {
			token = line.substr(0, pos);
			line.erase(0, pos + 1);
			size_t pos2 = 0;
			std::string token2;
			if ((pos2 = token.find(':')) != std::string::npos) {
				token2 = token.substr(0, pos2);
				token.erase(0, pos2 + 1);
				feature = ::atol(token2.c_str());
				value = ::atof(token.c_str());
                features[sample].push_back(feature);
                vals[sample].push_back(value);
                if (value > max_val) max_val = value;
			} else {
				classification = ::atol(token.c_str());
                classes.push_back(classification);
			}
		}
        sample++;
	}

    if (max_val > 1.0){
        // Normalization required for data set
        double normalized_val;
        for (i = 0; i < features.size(); i++) {
            for (long j = 0; j < features[i].size(); j++) {
                normalized_val = vals[i][j] / max_val;
                printf("%ld,%ld,%.10f,%ld\n", i, features[i][j], normalized_val, classes[i]);
                fflush(stdout);
            }
        }
    } else {
        for (i = 0; i < features.size(); i++) {
            for (long j = 0; j < features[i].size(); j++) {
                printf("%ld,%ld,%.10f,%ld\n", i, features[i][j], vals[i][j], classes[i]);
                fflush(stdout);
            }
        }
    }
}
//