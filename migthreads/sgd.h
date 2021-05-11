//
// Created by Brian Page on 2020-04-09.
//

#ifndef EMUSGD_SGD_H
#define EMUSGD_SGD_H

#include "../utility/emu.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "fcntl.h"
#include "time.h"
#include "stdint.h"
#include <math.h>

// Data allocation and misc globals
extern replicated long* working_vector;        // working vector for each nodelet
extern replicated long* training_sample_indicies; // training data sample id's
extern replicated long* training_feature_indicies;// training data feature id's (1:1 relationship to values vector)
extern replicated long* training_values;          // training data values (non-zero values for all samples)
extern replicated long* training_classifications; // training sample classes (single value for each sample)
extern replicated long* deg_reciprocal;

// replicated globals
extern replicated long cluster_count;           // num of clusters per simulation (M)
extern replicated long nodeletCount;            // number of nodelets used on the EMU system
extern replicated long threads_per_cluster;     // number of threads per cluster
extern replicated long total_epochs;            // num of epochs to run on each cluster
extern replicated long samples_per_epoch;       // num samples to evaluate per epoch
extern replicated long total_train_points;      // non-zeros in training data set
extern replicated long train_sample_count;      // samples in training data set (equivalent to row count in matrix)
extern replicated long featureSetSize;          // num features in training data set (equivalent to col count in matrix)
extern replicated long sample_start;
extern replicated long sample_end;

void train(long i, long scalar, long threads);

#endif //EMUSGD_SGD_H