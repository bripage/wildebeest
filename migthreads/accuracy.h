//
// Created by Brian Page on 2020-07-10.
//

#ifndef EMUSGD_ACCURACY_H
#define EMUSGD_ACCURACY_H

#include "emu.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "fcntl.h"
#include <string.h>
#include <math.h>

FILE * test_features;
FILE * test_labels;
extern char* test_feature_path;
extern char* test_label_path;
extern replicated long total_test_points;
extern replicated long test_sample_count;
extern replicated long* test_sample_indicies;
extern replicated long* test_feature_indicies;
extern replicated long* test_values;
extern replicated long* test_classifications;
extern replicated long cluster_count ;           // num of clusters per simulation (M)
extern replicated long nodeletCount;            // number of nodelets used on the EMU system
extern replicated long* working_vector;        // working vector for each nodelet
extern long nodelet_count_start;
extern long non_standard_classes;
extern long class1;
extern long class2;

void populateTestData();
double getModelAccuracy();


#endif //EMUSGD_ACCURACY_H
