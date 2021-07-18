//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_WILDEBEEST_H
#define EMUSGD_WILDEBEEST_H
#include "emu.h"
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"

/** Replicated global variables for training or execution flow */
replicated long threads;                 /** number of threads used for training */
replicated long gamma;                   /** step_decay - updates every epoch */
replicated long eta;                     /** step_size - remains constant */
replicated long total_epochs;            /** num of epochs to run */
replicated long featureSetSize;          /** Maximum possible features for an arbitrary sample */
replicated long train_sample_count;      /** number of samples in training data */
replicated long total_train_points;      /** Number of training data points in the data set */
replicated long test_sample_count;       /** Number of test data samples */
replicated long total_test_points;       /** Number of test data points */

/** stripped arrays pointers for allocating memory to hold training and testing data */
replicated long* model_vector;              /** model vector to be updated during training */
replicated long* training_sample_indicies;  /** starting element index into training_values for given sample */
replicated long* training_feature_indicies; /** training data feature id's (1:1 relationship to values vector) */
replicated long* training_values;           /** training data values (non-zero values for all samples) */
replicated long* training_classifications;  /** training sample classes (single value for each sample) */
replicated long* test_sample_indicies;      /** test sample element index into test_values */
replicated long* test_feature_indicies;     /** feature id's (1:1 relationship to values vector) */
replicated long* test_values;               /** test data for all samples */
replicated long* test_classifications;      /** known classification for each test sample */
replicated long* deg_reciprocal;            /** number of occurances for each feature (needed for training) */

#endif //EMUSGD_WILDEBEEST_H