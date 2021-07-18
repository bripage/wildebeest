//
// Created by Brian Page on 2020-07-10.
//

#include "accuracy.h"

/**
 * populateTestData() reads in test sample data for the current data set, in binary format as output by the csv2bin
 * utility. Similar to training data, test data is stored as three arrays: sample_indicies, feature_indicies, values.
 * Test data is stripped across all nodes in the system in the same manner as training data.
 */

void populateTestData() {
    long i, j = 0, current_sample = -1, fixed_value;
    long sample = -1, feature, class;
    test_features = NULL;
    test_features = fopen(test_feature_path, "rb");
    if (test_features == NULL) {
        printf("Failed to open test feature file.\n");
        exit(1);
    }
    long non_zeros = total_test_points - test_sample_count;
    long points;
    long *binBuffer;
    long bytesRead;
    points = non_zeros * 4;
    binBuffer = (long *) malloc(points * sizeof(long));
    bytesRead = fread(binBuffer, sizeof(long), points, test_features);
    if (bytesRead != (points)) {
        printf("*** Test Feature File Read Failure ***\n");
        exit(1);
    }
    
    for (i = 0; i < points; i += 4) {
        sample = binBuffer[i];
        feature = binBuffer[i + 1];
        fixed_value = binBuffer[i + 2];
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
            test_values[j] = fixed_value;
            test_classifications[sample] = class;
            current_sample = sample;
        } else {
            test_feature_indicies[j] = feature;
            test_values[j] = fixed_value;
        }
        j++;
    }
    test_sample_indicies[sample + 1] = j; // add sample id end ptr

    fclose(test_features);
    free(binBuffer);
}

/**
 * getModelAccuracy() evaluates a model vector against the testing data for the given data set. Testing is essentially
 * the same as training however we are now evaluating the accuracy of our model based on the number of correctly
 * predicted classifications (known for the test samples).
 */
double getModelAccuracy(){
	double correct_samples = 0.0, accuracy;
    long j, l_temp, dotProduct;
    long start, stop, feature;

	for (long i = 0; i < test_sample_count; i++) {
        dotProduct = 0;
        start = test_sample_indicies[i];
        stop = test_sample_indicies[i+1];

        for (j = start; j < stop; j++) {
            feature = test_feature_indicies[j];
            l_temp = test_values[j] * model_vector[feature];
            l_temp >>= 24;
            dotProduct += l_temp;
        }

        if (dotProduct * test_classifications[i] > 0){
            correct_samples += 1;
        }
	}

	accuracy = 100*(correct_samples/test_sample_count);
	return accuracy;
}