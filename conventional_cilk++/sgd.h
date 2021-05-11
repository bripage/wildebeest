//
// Created by Brian Page on 2020-04-09.
//

#ifndef EMUSGD_SGD_H
#define EMUSGD_SGD_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Must have this!
#include <math.h>
#include "omp.h"

// Data allocation and misc globals
extern double* working_vector;        // working vector for each nodelet
extern int64_t* training_sample_indicies; // training data sample id's
extern int64_t* training_feature_indicies;// training data feature id's (1:1 relationship to values vector)
extern double* training_values;          // training data values (non-zero values for all samples)
extern int64_t* training_classifications; // training sample classes (single value for each sample)

// replicated globals
extern int64_t total_epochs;            // num of epochs to run on each cluster
extern int64_t train_sample_count;      // samples in training data set (equivalent to row count in matrix)
extern double* deg_reciprocal;
extern double initial_step_size;
extern int64_t regularization_scalar;
extern int64_t featureSetSize;

void train(int64_t i, double scalar, int64_t threads);
#endif //EMUSGD_SGD_H