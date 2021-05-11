//
// Created by Brian Page on 2020-07-10.
//

#include "accuracy.h"

void populateTestData() {
    int64_t i;
    int64_t sample = -1;
    int64_t feature;
    int64_t fixed_value;
    int64_t class;
    double float_value;
    int64_t j = 0;
    int64_t sample_count = -1;

    int64_t current_sample = -1;
    test_features = NULL;
    test_features = fopen(test_feature_path, "rb");
    if (test_features == NULL) {
        printf("Failed to open test feature file.\n");
        exit(1);
    }

    int64_t non_zeros = total_test_points - test_sample_count;
    int64_t points;
    int64_t *binBuffer;
    int64_t bytesRead;

    points = non_zeros * 4;
    printf("points = %ld\n", points);
    fflush(stdout);
    binBuffer = (int64_t *) malloc(points * sizeof(int64_t));
    bytesRead = fread(binBuffer, sizeof(int64_t), points, test_features);
    if (bytesRead != (points)) {
        printf("*** Test Feature File Read Failure ***\n");
        exit(1);
    }
    printf("bytesRead = %ld\n", bytesRead);
    fflush(stdout);
    for (i = 0; i < points; i += 4) {
        //printf("%ld\n", i);
        //fflush(stdout);
        sample = binBuffer[i];
        feature = binBuffer[i + 1];
        fixed_value = binBuffer[i + 2];
        float_value = (double) fixed_value;
        float_value /= 16777216;
        class = binBuffer[i + 3];

        if (non_standard_classes) {
            if (class == class1) {
                class = -1;
            } else if (class == class2) {
                class = 1;
            } else {
                printf("ERROR: Training Data classes do not match class range\n");
                fflush(stdout);
                exit(2);
            }
        }

        if (sample != current_sample) {
            test_feature_indicies[j] = 0;
            test_values[j] = 1;
            j++;
            test_sample_indicies[sample] = j;
            test_feature_indicies[j] = feature;
            test_values[j] = float_value;
            test_classifications[sample] = class;
            current_sample = sample;
        } else {
            test_feature_indicies[j] = feature;
            test_values[j] = float_value;
        }
        j++;
    }
    test_sample_indicies[sample + 1] = j; // add sample id end ptr

    fclose(test_features);
    free(binBuffer);
}

double getModelAccuracy(){
	double correct_samples = 0.0;
	double accuracy;
    int64_t j;
    double dotProduct;
    int64_t start;
    int64_t stop;
    int64_t feature;
    double d_temp;
    long negatives = 0;

	for (int64_t i = 0; i < test_sample_count; i++) {
        dotProduct = 0.0;
        start = test_sample_indicies[i];
        stop = test_sample_indicies[i+1];

        for (j = start; j < stop; j++) {
            feature = test_feature_indicies[j];
            dotProduct += test_values[j] * working_vector[feature];
        }
        if (dotProduct < 0) negatives++;

        if (dotProduct * test_classifications[i] > 0){
            correct_samples += 1;
        }
	}

	accuracy = 100*(correct_samples/test_sample_count);
	return accuracy;
}