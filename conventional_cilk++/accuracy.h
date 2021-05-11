//
// Created by Brian Page on 2020-07-10.
//

#ifndef EMUSGD_ACCURACY_H
#define EMUSGD_ACCURACY_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Must have this!
#include <math.h>

FILE * test_features;
FILE * test_labels;
extern char* test_feature_path;
extern char* test_label_path;
extern int64_t total_test_points;
extern int64_t test_sample_count;
extern int64_t* test_sample_indicies;
extern int64_t* test_feature_indicies;
extern double* test_values;
extern int64_t* test_classifications;
extern double* working_vector;        // working vector for each nodelet
extern int64_t non_standard_classes;
extern int64_t class1;
extern int64_t class2;

void populateTestData();
double getModelAccuracy();


#endif //EMUSGD_ACCURACY_H
