/*
 * Shuffle: This utility program will read in an input file, randomly permute its contents line by line, and then
 *          output the result to the console.
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

	for (int i = 1; i < argc; i= i+2){
		argTemp = argv[i];
		if (argTemp == "-i"){
			// load data file.
			inputDataFile = argv[i+1];
		} else if (argTemp == "-help" || argTemp == "--help" || argTemp == "-h"){
            printf("Shuffle: This utility program will read in an input file, randomly permute its contents"
                   "(by line), and then output the result to the console.\n\n");
            printf("Usage: ./shuffle [OPTION] <argument> ...\n");
            printf("Options:\n");
            printf("-i <file> \tIn put filename and path\n");
            exit(0);
		} else {
			printf("%s Is not a valid parameter. Try --help for more information.\n EXITING!\n", argv[i]);
			exit(0);
		}
	}

	// Read in file
	std::ifstream infile(inputDataFile);
	if (!infile) {
		std::cout << "FAILED TO OPEN FILE!" << std::endl;
		exit(1);
	}

	std::string line;
    std::vector<std::string> lines;
	while (std::getline(infile, line)) {
		lines.push_back(line);
	}

    std::vector<long> lineOrder;
    for (long i = 0; i < lines.size(); i++){
        lineOrder.push_back(i);
    }
    std::random_shuffle (lineOrder.begin(), lineOrder.end());

	long curSample;
	for (long i = 0; i < lines.size(); i++) {
        curSample = lineOrder[i];
        std::cout << lines[curSample] << std::endl;
    }
}