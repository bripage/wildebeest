//
// Created by Brian Page on 2020-04-09.
//

#include "sgd.h"

void train(int64_t i, double scalar, int64_t threads) {
    int64_t j;
    int64_t start;
    int64_t stop;
    int64_t class;
    int64_t feature;
    double distance;
    double di;
    double wv_temp;
    double d_temp;

    int64_t sample = i;
    while (sample < train_sample_count) {
        distance = 0;
        start = training_sample_indicies[sample];
        stop = training_sample_indicies[sample + 1];
        class = training_classifications[sample];
        for (j = start; j < stop; j++) {
            feature = training_feature_indicies[j];
            distance += training_values[j] * working_vector[feature];
        }
        distance *= class;
        //printf("distance = %lf\n", distance);
        //fflush(stdout);

        if (distance < 1.0) {
            di = scalar * class;
            for (j = start; j < stop; j++) {
                feature = training_feature_indicies[j];
                d_temp = di * training_values[j];
                working_vector[feature] += d_temp;

                d_temp = (scalar*regularization_scalar) / deg_reciprocal[feature];
                wv_temp = (1 - d_temp);
                working_vector[feature] *= wv_temp;
            }
        } else {
            for (j = start; j < stop; j++) {
                feature = training_feature_indicies[j];
                d_temp = (scalar*regularization_scalar) / deg_reciprocal[feature];
                wv_temp = (1 - d_temp);
                working_vector[feature] *= wv_temp;
            }
        }
        sample += threads;
    }
}