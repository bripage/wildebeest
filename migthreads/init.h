//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_INIT_H
#define EMUSGD_INIT_H
#include "../utility/emu.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "fcntl.h"
#include <string.h>
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
replicated long compute_accuracy;        // accuracy computation flag
long nodelet_count_start;
long thread_start;
long nodelet_count_end;
long thread_end;
long non_standard_classes;
long class1;
long class2;
long get_epoch_accuracy;

/// Data allocation and misc globals
replicated long* working_vector;        // working vector for each nodelet
replicated long* training_sample_indicies; // training data sample id's
replicated long* training_feature_indicies;// training data feature id's (1:1 relationship to values vector)
replicated long* training_values;          // training data values (non-zero values for all samples)
replicated long* training_classifications; // training sample classes (single value for each sample)
replicated long* test_sample_indicies;
replicated long* test_feature_indicies;
replicated long* test_values;
replicated long* test_classifications;
replicated long* deg_reciprocal;

/// Replicated globals
replicated long cluster_count;              // num of clusters per simulation (M)
replicated long nodeletCount;               // number of nodelets used on the EMU system
replicated long threads_per_cluster;        // number of threads per cluster
replicated long total_epochs;               // num of epochs to run on each cluster
replicated long samples_per_epoch;          // num samples to evaluate per epoch
replicated long gamma;                      // initial step decay
replicated long eta;                        // initial step size
replicated long total_train_points;         // non-zeros in training data set
replicated long train_sample_count;         // samples in training data set (equivalent to row count in matrix)
replicated long featureSetSize;             // num features in training data set (equivalent to col count in matrix)
replicated long total_test_points;
replicated long test_sample_count;
replicated long sample_start;
replicated long sample_end;

void parse_args(int argc, char * argv[]);
void init();
void init_mem(long n);
void reset_mem(long n);
void reset_cluster_count(long n, long nodelet_count, long samples_per_nodelet);

#endif //EMUSGD_INIT_H