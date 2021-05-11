//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_INIT_H
#define EMUSGD_INIT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Must have this!
#include <math.h>

/// Temporary variables and execution behavior flags
double initial_step_size;                // becomes eta
double initial_step_decay;               // becomes gamma
FILE * train_data;                       // training data file ptr
FILE * class_data;                       // training data sample class file ptr
char * train_data_path;                  // path to training data file
char * class_data_path;                  // path to training sample class file
char* test_feature_path;                 // path to accuracy testing file
char* test_label_path;                   // path to accuracy testing sample class file
int64_t compute_accuracy;        // accuracy computation flag
int64_t nodelet_count_start;
int64_t thread_start;
int64_t nodelet_count_end;
int64_t thread_end;
int64_t non_standard_classes;
int64_t class1;
int64_t class2;
int64_t regularization_scalar;
int64_t get_epoch_accuracy;

/// Data allocation and misc globals
double* working_vector;        // working vector for each nodelet
int64_t* training_sample_indicies; // training data sample id's
int64_t* training_feature_indicies;// training data feature id's (1:1 relationship to values vector)
double* training_values;          // training data values (non-zero values for all samples)
int64_t* training_classifications; // training sample classes (single value for each sample)
int64_t* test_sample_indicies;
int64_t* test_feature_indicies;
double* test_values;
int64_t* test_classifications;
double* deg_reciprocal;

/// Replicated globals
int64_t total_epochs;               // num of epochs to run on each cluster
double gama;                      // initial step decay
double eta;                        // initial step size
int64_t total_train_points;         // non-zeros in training data set
int64_t train_sample_count;         // samples in training data set (equivalent to row count in matrix)
int64_t featureSetSize;             // num features in training data set (equivalent to col count in matrix)
int64_t total_test_points;
int64_t test_sample_count;


void parse_args(int argc, char * argv[]);
void init();
void init_mem(int64_t n);

#endif //EMUSGD_INIT_H