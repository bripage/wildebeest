//
// Created by bpage1 on 4/3/20.
// Last Updated on 6/19/2020
//
#include "fcntl.h"
#include "stdlib.h"
#include "stdint.h"
#include "hogwild.h"
#include "sgd.h"
#include "accuracy.h"

#include <sys/time.h>

int main(int argc, char **argv) {
    parse_args(argc, argv);
    init();

    volatile double start_time, total_time, execution_time;
    double accuracy = 0.0;
    double scalar = eta;

    if (!get_epoch_accuracy) {
        struct timeval tval_before, tval_after, tval_result;

        for (uint64_t threads = thread_start; threads <= thread_end; threads *= 2) {
            scalar = eta;
            gettimeofday(&tval_before, NULL);
            for (uint64_t epoch = 1; epoch <= total_epochs; epoch++) {
                if (epoch > 1) {
                    scalar *= gama;
                }

                for (long i = 0; i < threads; i++) {
                    cilk_spawn train(i, scalar, threads);
                }
                cilk_sync;

                //accuracy = getModelAccuracy();
                //printf("epoch %ld: %lf\n", epoch, accuracy);
                //fflush(stdout);
            }
            gettimeofday(&tval_after, NULL);
            double msec_before = (tval_before.tv_sec * 1000) + tval_before.tv_usec;
            double msec_after = (tval_after.tv_sec * 1000) + tval_after.tv_usec;
            long usec_diff = (tval_after.tv_sec - tval_before.tv_sec)*1000000 + (tval_after.tv_usec - tval_before.tv_usec);

            if (compute_accuracy) {
                accuracy = getModelAccuracy();
                printf("%ld,%lf,%ld\n", threads, accuracy, usec_diff/1000);
                fflush(stdout);
            } else {
                printf("%ld,--,%ld\n", threads, usec_diff/1000);
                fflush(stdout);
            }

            for (uint64_t i = 0; i < featureSetSize; i++) {
                working_vector[i] = 0;
            }
        }
    } else {
        for (uint64_t threads = thread_start; threads <= thread_end; threads *= 2) {
            printf("%ld Threads\n", threads);
            scalar = eta;
            for (uint64_t epoch = 1; epoch <= total_epochs; epoch++) {
                if (epoch > 1) {
                    scalar *= gama;
                }

                for (long i = 0; i < threads; i++) {
                    cilk_spawn train(i, scalar, threads);
                }
                cilk_sync;

                accuracy = getModelAccuracy();
                printf("epoch %ld: %lf\n", epoch, accuracy);
                fflush(stdout);
            }

            for (uint64_t i = 0; i < featureSetSize; i++) {
                working_vector[i] = 0;
            }
        }
    }


	printf("Done\n");
	fflush(stdout);
	return 0;
}