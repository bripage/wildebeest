//
// Created by bpage1 on 4/3/20.
// Last Updated on 6/19/2020
//
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"
#include "wildebeest.h"
#include "sgd.h"
#include "accuracy.h"

int main(int argc, char **argv) {
    parse_args(argc, argv);
    init();

    volatile double start_time, total_time, execution_time;
    double accuracy = 0.0;
    double scalar = eta;
    double gamma_temp = gamma;
    uint64_t thread_id;

    if (!get_epoch_accuracy) {
        for (uint64_t threads = thread_start; threads <= thread_end; threads *= 2) {
            scalar = eta;
            start_time = omp_get_wtime();
            for (uint64_t epoch = 1; epoch <= total_epochs; epoch++) {
                if (epoch > 1) {
                    scalar *= gamma_temp;
                }

                #pragma omp parallel num_threads(threads) shared(threads, scalar, train_sample_count, working_vector, deg_reciprocal, training_sample_indicies, training_feature_indicies, training_values, training_classifications) private(thread_id)
                {
                    thread_id = omp_get_thread_num();
                    train(thread_id, scalar, threads);
                }
            }
            total_time = omp_get_wtime() - start_time;
            execution_time = total_time * 1000;

            if (compute_accuracy) {
                accuracy = getModelAccuracy();
                printf("%ld,%lf,%lf\n", threads, accuracy, execution_time);
                fflush(stdout);
            } else {
                printf("%ld,%lf,%lf\n", threads, 0.0, execution_time);
                fflush(stdout);
            }

            for (uint64_t i = 0; i < featureSetSize; i++) {
                working_vector[i] = 0;
            }
        }
    } else {
        for (uint64_t threads = thread_start; threads <= thread_end; threads *= 2) {
            scalar = eta;
            for (uint64_t epoch = 1; epoch <= total_epochs; epoch++) {
                if (epoch > 1) {
                    scalar *= gamma_temp;
                }

                #pragma omp parallel num_threads(threads) shared(threads, scalar, train_sample_count, working_vector, deg_reciprocal, training_sample_indicies, training_feature_indicies, training_values, training_classifications) private(thread_id)
                {
                    thread_id = omp_get_thread_num();
                    train(thread_id, scalar, threads);
                }

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