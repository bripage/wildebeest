//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_WILDEBEEST_H
#define EMUSGD_WILDEBEEST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Must have this!
#include <cilk/cilk.h>

/// Temporary variables and execution behavior flags
extern int64_t compute_accuracy;        // accuracy computation flag
extern int64_t thread_start;
extern int64_t thread_end;
extern double initial_step_size;                // becomes eta
extern double initial_step_decay;               // becomes gamma
extern int64_t samples_per_epoch;       // num samples to evaluate per epoch
extern int64_t non_standard_classes;
extern int64_t class1;
extern int64_t class2;

/// Data allocation and misc globals
extern double* working_vector;        // working vector for each nodelet
extern double* deg_reciprocal;
extern int64_t* training_sample_indicies; // training data sample id's
extern int64_t* training_feature_indicies;// training data feature id's (1:1 relationship to values vector)
extern double* training_values;          // training data values (non-zero values for all samples)
extern int64_t* training_classifications; // training sample classes (single value for each sample)
extern int64_t train_sample_count;
extern double gama;                      // initial step decay
extern double eta;                        // initial step size
extern int64_t featureSetSize;
extern int64_t get_epoch_accuracy;

#endif //EMUSGD_WILDEBEEST_H