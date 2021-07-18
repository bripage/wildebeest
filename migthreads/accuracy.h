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

/** local/temporary variables */
FILE * test_features;
FILE * test_labels;
extern char* test_feature_path;
extern char* test_label_path;

/** Replicated global variables for training or execution flow */
extern long non_standard_classes;                       /** flag indicating training data's class values are not 0 or 1 */
extern long class1, class2;                             /** The class values present in the training data set to be updated */
extern replicated long test_sample_count;        /** Number of test data samples */
extern replicated long total_test_points;        /** Number of test data points */

/** stripped arrays pointers for allocating memory to hold training and testing data */
extern replicated long* model_vector;              /** model vector to be updated during training */
extern replicated long* training_sample_indicies;  /** starting element index into training_values for given sample */
extern replicated long* training_feature_indicies; /** training data feature id's (1:1 relationship to values vector) */
extern replicated long* training_values;           /** training data values (non-zero values for all samples) */
extern replicated long* training_classifications;  /** training sample classes (single value for each sample) */
extern replicated long* test_sample_indicies;      /** test sample element index into test_values */
extern replicated long* test_feature_indicies;     /** feature id's (1:1 relationship to values vector) */
extern replicated long* test_values;               /** test data for all samples */
extern replicated long* test_classifications;      /** known classification for each test sample */
extern replicated long* deg_reciprocal;            /** number of occurances for each feature (needed for training) */

void populateTestData();
double getModelAccuracy();


#endif //EMUSGD_ACCURACY_H
