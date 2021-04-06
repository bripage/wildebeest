//
// Created by bpage1 on 4/3/20.
// Last Updated on 6/19/2020
//
#include "../utility/emu.h"
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"
#include "hogwild.h"
//#include "init.h"
#include "sgd.h"
#include "accuracy.h"

int main(int argc, char **argv) {
    parse_args(argc, argv);
    mw_replicated_init(&sample_start, 0);
    mw_replicated_init(&sample_end, 0);
    init();

    MIGRATE(&working_vector[0]);
    volatile uint64_t start_time, total_time;
    volatile double execution_time;
    double accuracy = 0.0;
    long scalar = eta;
    long gamma_temp = gamma;

    for (long threads = thread_start; threads <= thread_end; threads *= 2) {
        start_time = CLOCK();
        for (long epoch = 1; epoch <= total_epochs; epoch++) {
            if (epoch > 1) {
                scalar *= gamma_temp;
                scalar >>= 24;
            }
            for (long k = 0; k < threads; k++) {
                cilk_migrate_hint(&training_sample_indicies[k]);
                cilk_spawn train(k, threads, scalar);
            }
            cilk_sync;
        }
        total_time = CLOCK() - start_time;
        execution_time = (double) total_time / 175000000;

        if (compute_accuracy) {
            accuracy = getModelAccuracy();
            printf("%ld,%f,%lf\n", threads, accuracy, execution_time * 1000);
            fflush(stdout);
        } else {
            printf("%ld,%lf,%lf\n", threads, 0.0, execution_time * 1000);
            fflush(stdout);
        }

        for (long i = 0; i < featureSetSize; i++) {
            working_vector[i] = 0;
        }
    }

	printf("Done\n");
	fflush(stdout);
	return 0;
}