//
// Created by Brian Page on 2020-04-09.
//

#include "sgd.h"

void train(long i, long threads, long scalar) {
    DISABLE_ACKS();
    long j;
    long start;
    long stop;
    long class;
    long feature;
    long distance;
    long di;
    long wv_temp;
    long l_temp;

    long sample = i;
    while (sample < train_sample_count) {
        distance = 0;
        start = training_sample_indicies[sample];
        stop = training_sample_indicies[sample + 1];
        class = training_classifications[sample];
        for (j = start; j < stop; j++) {
            feature = training_feature_indicies[j];
            l_temp = training_values[j] * working_vector[feature];
            l_temp >>= 24;
            distance += l_temp;
        }
        distance *= class;

        if (distance < 16777216) {
            di = scalar * class;
            for (j = start; j < stop; j++) {
                feature = training_feature_indicies[j];
                l_temp = (di * training_values[j]) >> 24;
                wv_temp = working_vector[feature];
                wv_temp += l_temp;
                l_temp = (scalar * deg_reciprocal[feature]) >> 24;
                wv_temp = (wv_temp * (16777216 - l_temp)) >> 24;
                working_vector[feature] = wv_temp;
            }
        } else {
            for (j = start; j < stop; j++) {
                feature = training_feature_indicies[j];
                wv_temp = working_vector[feature];
                l_temp = (scalar * deg_reciprocal[feature]) >> 24;
                wv_temp = (wv_temp * (16777216 - l_temp)) >> 24;
                working_vector[feature] = wv_temp;
            }
        }
        //printf("sample = %ld\n", sample);
        //fflush(stdout);
        sample += threads;
    }
}