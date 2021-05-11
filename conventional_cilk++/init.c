//
// Created by bpage1 on 4/3/20.
//
// 2^22 = 4194304
// 2^24 = 16777216
//

#include "init.h"
#include "accuracy.h"

void parse_args(int argc, char * argv[]) {
    int64_t num_arg;
    int64_t i;
    double scaled_float;
    train_data_path = NULL;
    test_feature_path = NULL;
    compute_accuracy = 0;
    thread_start = 1;
    thread_end = 1;
    non_standard_classes = 0;
    get_epoch_accuracy = 0;

    for (i = 1; i < argc; i++){
        if (!strcmp(argv[i],"--train-data")){
            train_data_path = (char *) malloc(strlen(argv[i+1]) * sizeof(char));
            strcpy(train_data_path, argv[i+1]);
            printf("train_data_path = %s\n",train_data_path);
            fflush(stdout);
        } else if (!strcmp(argv[i],"--test-data")){
            test_feature_path = (char *) malloc(strlen(argv[i+1]) * sizeof(char));
            strcpy(test_feature_path, argv[i+1]);
            printf("test_data_path = %s\n", test_feature_path);
            fflush(stdout);
        } else if (!strcmp(argv[i],"-f")){
            num_arg = atoi(argv[i+1]) + 1;
            featureSetSize = num_arg;
            printf("featureSetSize = %ld\n", featureSetSize);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--train-samples")){
            num_arg = atoi(argv[i+1]);
            train_sample_count = num_arg;
            printf("train_sample_count = %ld\n", train_sample_count);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--regularization-scalar")){
            num_arg = atoi(argv[i+1]);
            regularization_scalar = num_arg;
            printf("regularization_scalar = %ld\n", regularization_scalar);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--class-values")){
            num_arg = atoi(argv[i+1]);
            class1 = num_arg;
            num_arg = atoi(argv[i+2]);
            class2 = num_arg;
            printf("classes: {%ld,%ld}\n", class1, class2);
            fflush(stdout);
            non_standard_classes = 1;
            i+=2;
        } else if (!strcmp(argv[i],"--compute-accuracy")){
            compute_accuracy = 1;
            printf("Compute Accuracy ON\n");
            fflush(stdout);
        } else if (!strcmp(argv[i],"--get-epoch-accuracy")){
            get_epoch_accuracy = 1;
            printf("Get Epoch Accuracy TRUE\n");
            fflush(stdout);
        } else if (!strcmp(argv[i],"-e")){
            num_arg = atoi(argv[i+1]);
            total_epochs = num_arg;
            printf("total_epochs = %ld\n", total_epochs);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--train-points")){
            num_arg = atoi(argv[i+1])+train_sample_count;
            total_train_points = num_arg;
            printf("total_train_points = %ld\n", total_train_points);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--test-samples")){
	        num_arg = atoi(argv[i+1]);
	        test_sample_count = num_arg;
	        printf("test_sample_count = %ld\n", test_sample_count);
	        fflush(stdout);
	        i++;
        } else if (!strcmp(argv[i],"--test-points")){
	        num_arg = atoi(argv[i+1])+test_sample_count;
	        total_test_points = num_arg;
	        printf("total_test_points = %ld\n", total_test_points);
	        fflush(stdout);
	        i++;
        } else if (!strcmp(argv[i],"--nodelet-start")){
            num_arg = atoi(argv[i+1]);
            nodelet_count_start = num_arg;
            printf("Nodelet Count Start = %ld\n", nodelet_count_start);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--thread-start")){
            num_arg = atoi(argv[i+1]);
            thread_start = num_arg;
            printf("Threads Per Nodelet Start = %ld\n", thread_start);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--nodelet-count-end")){
            num_arg = atoi(argv[i+1]);
            nodelet_count_end = num_arg;
            printf("Nodelet Count End = %ld\n", nodelet_count_end);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--thread-end")){
            num_arg = atoi(argv[i+1]);
            thread_end = num_arg;
            printf("Threads Per Nodelet End = %ld\n", thread_end);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--initial-step-size")){
            sscanf(argv[i+1],"%lf",&scaled_float);
            eta = scaled_float;
            printf("initial step size: %lf\n", scaled_float);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--initial-step-decay")){
            sscanf(argv[i+1],"%lf", &scaled_float);
            gama = scaled_float;
            printf("initial_step_decay = %lf\n", scaled_float);
            fflush(stdout);
            i++;
        }
    }

    printf("--- Parsing Arguments Complete ---\n");
    fflush(stdout);
}

void populateTrainingData() {
    printf("inside populate_data()\n");
    fflush(stdout);

    int64_t i,
            sample = -1,
            feature,
            fixed_value,
            class;
    double float_value;
    int64_t j = 0;
    int64_t current_sample = -1;
    int64_t sample_count = -1;
    training_sample_indicies[0] = 0;

    train_data = NULL;
    train_data = fopen(train_data_path, "rb");
    if (train_data == NULL) {
        printf("Failed to open training feature file.\n");
        exit(1);
    }

    int64_t non_zeros = total_train_points - train_sample_count;
    int64_t points;
    int64_t *binBuffer;
    int64_t bytesRead;

    points = non_zeros * 4;
    printf("points = %ld\n", points);
    fflush(stdout);

        binBuffer = (int64_t *) malloc(points * sizeof(int64_t));
        bytesRead = fread(binBuffer, sizeof(int64_t), points, train_data);

        if (bytesRead != (points)) {
            printf("*** Feature File Read Failure ***\n");
            exit(1);
        }

        for (i = 0; i < points; i += 4) {
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
                sample_count++;
                //printf("Populating Sample %ld\n", sample_count);
                //fflush(stdout);
                training_feature_indicies[j] = 0;
                training_values[j] = 1;
                deg_reciprocal[0] += 1;
                j++;
                training_sample_indicies[sample_count] = j;
                training_feature_indicies[j] = feature;
                training_values[j] = float_value;
                training_classifications[sample_count] = class;
                deg_reciprocal[feature] += 1;
                current_sample = sample;
            } else {
                training_feature_indicies[j] = feature;
                training_values[j] = float_value;
                deg_reciprocal[feature] += 1;
            }
            j++;
        }
        training_sample_indicies[sample_count + 1] = j; // add sample id end ptr
        fclose(train_data);
        free(binBuffer);

        //for (int64_t i = 0; i <= featureSetSize; i++) {
        //    deg_reciprocal[i] = 1.0 / deg_reciprocal[i];
        //}

        printf("populate_data() done\n");
        fflush(stdout);
}

void init() {
    working_vector = (double *) malloc(featureSetSize * sizeof(double));
    deg_reciprocal = (double *) malloc(featureSetSize * sizeof(double));
    training_sample_indicies = (int64_t *) malloc((train_sample_count + 1) * sizeof(int64_t));
    training_feature_indicies = (int64_t *) malloc(total_train_points * sizeof(int64_t));
    training_values = (double *) malloc(total_train_points * sizeof(double));
    training_classifications = (int64_t *) malloc(train_sample_count * sizeof(int64_t));

    if (compute_accuracy) {
        test_sample_indicies = (int64_t *) malloc((test_sample_count + 1) * sizeof(int64_t));
        test_feature_indicies = (int64_t *) malloc(total_test_points * sizeof(int64_t));
        test_values = (double *) malloc(total_test_points * sizeof(double));
        test_classifications = (int64_t *) malloc(test_sample_count * sizeof(int64_t));
    }

    printf("--- Memmory Allocation Complete ---\n");
	fflush(stdout);

    for (int64_t i = 0; i < featureSetSize; i++) {
        working_vector[i] = 0.0;
        deg_reciprocal[i] = 0.0;
    }
    for (int64_t i = 0; i < train_sample_count; i++) {
        training_sample_indicies[i] = 0;
        training_classifications[i] = 0;
    }
    training_sample_indicies[train_sample_count] = 0;
    for (int64_t i = 0; i < total_train_points; i++) {
        training_feature_indicies[i] = 0;
        training_values[i] = 0.0;
    }

	printf("--- Memmory Initialization Complete ---\n");
	fflush(stdout);

	populateTrainingData();
    if (compute_accuracy) {
        populateTestData();
    }

	printf("--- Initialization Complete ---\n");
	fflush(stdout);
}
