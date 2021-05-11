//
// Created by bpage1 on 4/3/20.
//

#ifndef EMUSGD_WILDEBEEST_H
#define EMUSGD_WILDEBEEST_H
#include "../utility/emu.h"
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"

/// Temporary variables and execution behavior flags
extern replicated long compute_accuracy;        // accuracy computation flag
long nodelet_count_start;
long thread_start;
long nodelet_count_end;
long thread_end;
extern double initial_step_size;                // becomes eta
extern double initial_step_decay;               // becomes gamma
extern replicated long samples_per_epoch;       // num samples to evaluate per epoch
extern long non_standard_classes;
extern long class1;
extern long class2;
extern long get_epoch_accuracy;

/// Data allocation and misc globals
extern replicated long* working_vector;        // working vector for each nodelet
extern replicated long cluster_count;           // num of clusters per simulation (M)
extern replicated long nodeletCount;            // number of nodelets used on the EMU system
extern replicated long threads_per_cluster;     // number of threads per cluster
extern replicated long train_sample_count;
extern replicated long sample_start;
extern replicated long sample_stend;
extern replicated long gamma;                      // initial step decay
extern replicated long eta;                        // initial step size

#endif //EMUSGD_WILDEBEEST_H