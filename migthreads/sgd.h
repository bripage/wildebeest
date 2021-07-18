//
// Created by Brian Page on 2020-04-09.
//

#ifndef EMUSGD_SGD_H
#define EMUSGD_SGD_H

#include "emu.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "fcntl.h"
#include "time.h"
#include "stdint.h"
#include <math.h>

/** Replicated global variables for training or execution flow */
extern replicated long threads;                 /** number of threads used for training */

/** stripped arrays pointers for allocating memory to hold training and testing data */
extern replicated long* model_vector;              /** model vector to be updated during training */
extern replicated long* training_sample_indicies;  /** starting element index into training_values for given sample */
extern replicated long* training_feature_indicies; /** training data feature id's (1:1 relationship to values vector) */
extern replicated long* training_values;           /** training data values (non-zero values for all samples) */
extern replicated long* training_classifications;  /** training sample classes (single value for each sample) */
extern replicated long* deg_reciprocal;            /** number of occurances for each feature (needed for training) */

void train(long i, long scalar, long threads);

#endif //EMUSGD_SGD_H