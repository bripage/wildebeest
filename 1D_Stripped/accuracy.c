//
// Created by Brian Page on 2020-07-10.
//

#include "accuracy.h"

void populateTestData() {
	long i;
	long sample = -1;
	long feature;
	long fixed_value;
	long class;
	long j = 0;
    long sample_count = -1;

	long current_sample = -1;
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

    if (non_zeros > 10000000) {
        long chunk_count;
        if (non_zeros % 10000000 != 0) {
            chunk_count = (non_zeros / 10000000) + 1;
        } else {
            chunk_count = non_zeros / 10000000;
        }
        printf("chunk_count = %ld\n", chunk_count);
        fflush(stdout);

        binBuffer = (long *) malloc(10000000 * sizeof(long));
        for (long chunk = 0; chunk < chunk_count; chunk++) {
            if (chunk != chunk_count - 1) {
                points = 10000000;
                printf("1 chunk %ld, points = %ld\n", chunk, points);
                fflush(stdout);
            } else {
                points = non_zeros - (chunk * 10000000);
                free(binBuffer);
                binBuffer = (long *) malloc(points * sizeof(long));
                printf("2 chunk %ld, points = %ld\n", chunk, points);
                fflush(stdout);
            }
            bytesRead = fread(binBuffer, sizeof(long), points, test_features);
            if (bytesRead != (points)) {
                printf("bytesRead = %ld, points = %ld\n", bytesRead, points);
                fflush(stdout);
                printf("***  File Read Failure ***\n");
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
                    sample_count++;
                    test_feature_indicies[j] = 0;
                    test_values[j] = 1;
                    j++;
                    test_sample_indicies[sample_count] = j;
                    test_feature_indicies[j] = feature;
                    test_values[j] = fixed_value;
                    test_classifications[sample_count] = class;
                    current_sample = sample;
                } else {
                    test_feature_indicies[j] = feature;
                    test_values[j] = fixed_value;
                }
                j++;
            }
        }
        test_sample_indicies[sample_count + 1] = j; // add sample id end ptr
    } else {
        points = non_zeros * 4;
        printf("points = %ld\n", points);
        fflush(stdout);

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
    }

	fclose(test_features);
	free(binBuffer);
}

double getModelAccuracy(){
	double correct_samples = 0.0;
	double accuracy;
    long j;
    long dotProduct;
    long start;
    long stop;
    long feature;
    long l_temp;

	for (long i = 0; i < test_sample_count; i++) {
        dotProduct = 0;
        start = test_sample_indicies[i];
        stop = test_sample_indicies[i+1];

        for (j = start; j < stop; j++) {
            feature = test_feature_indicies[j];
            l_temp = test_values[j] * working_vector[feature];
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