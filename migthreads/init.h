//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_INIT_H
#define EMUSGD_INIT_H
#include "emu.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "fcntl.h"
#include <string.h>
#include <math.h>

/** local/temporary variables */
FILE * train_data;                       /** training data file ptr */
FILE * class_data;                       /** training data sample class file ptr */
char * train_data_path;                  /** path to training data file */
char * class_data_path;                  /** path to training sample class file */
char* test_feature_path;                 /** path to accuracy testing file */
char* test_label_path;                   /** path to accuracy testing sample class file */
double initial_step_size;                /** becomes eta */
double initial_step_decay;               /** becomes gamma */

/** Replicated global variables for training or execution flow */
long non_standard_classes;               /** flag indicating training data's class values are not 0 or 1 */
long class1, class2;                     /** The class values present in the training data set to be updated */
extern replicated long threads;                 /** number of threads used for training */
extern replicated long gamma;                   /** step_decay - updates every epoch */
extern replicated long eta;                     /** step_size - remains constant */
extern replicated long total_epochs;            /** num of epochs to run */
extern replicated long featureSetSize;          /** Maximum possible features for an arbitrary sample */
extern replicated long train_sample_count;      /** number of samples in training data */
extern replicated long total_train_points;      /** Number of training data points in the data set */
extern replicated long test_sample_count;       /** Number of test data samples */
extern replicated long total_test_points;       /** Number of test data points */

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

void init();
#endif //EMUSGD_INIT_H