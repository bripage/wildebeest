//
// Created by bpage1 on 4/3/20.
//
// 2^22 = 4194304
// 2^24 = 16777216
//

#include "init.h"
#include "accuracy.h"

/** pars_args() - reads in command line parameters for data set characteristics as well as system size. Replicated
 * variables are used where appropriate to insure thread migrations do not occur during execution by creating a local
 * copy on every node.
 *
 * @param argc
 * @param argv
 */
void parse_args(int argc, char * argv[]) {
    long num_arg;
    long i;
    double scaled_float;
    train_data_path = NULL;
    test_feature_path = NULL;
    non_standard_classes = 0;

    for (i = 1; i < argc; i++){
        if (!strcmp(argv[i],"--trainers")){
            num_arg = atoi(argv[i+1]) + 1;
            mw_replicated_init(&threads, num_arg);
            printf("Trainer Count = %d\n",threads);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--train-data")){
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
            mw_replicated_init(&featureSetSize, num_arg);
            printf("featureSetSize = %ld\n", featureSetSize);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--train-samples")){
            num_arg = atoi(argv[i+1]);
            mw_replicated_init(&train_sample_count, num_arg);
            printf("train_sample_count = %ld\n", train_sample_count);
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
        } else if (!strcmp(argv[i],"-e")){
            num_arg = atoi(argv[i+1]);
            mw_replicated_init(&total_epochs, num_arg);
            printf("total_epochs = %ld\n", total_epochs);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--train-points")){
            num_arg = atoi(argv[i+1])+train_sample_count;
            mw_replicated_init(&total_train_points, num_arg);
            printf("total_train_points = %ld\n", total_train_points);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--test-samples")){
	        num_arg = atoi(argv[i+1]);
	        mw_replicated_init(&test_sample_count, num_arg);
	        printf("test_sample_count = %ld\n", test_sample_count);
	        fflush(stdout);
	        i++;
        } else if (!strcmp(argv[i],"--test-points")){
	        num_arg = atoi(argv[i+1])+test_sample_count;
	        mw_replicated_init(&total_test_points, num_arg);
	        printf("total_test_points = %ld\n", total_test_points);
	        fflush(stdout);
	        i++;
        } else if (!strcmp(argv[i],"--initial-step-size")){
            sscanf(argv[i+1],"%lf",&scaled_float);
            double d_temp = scaled_float * 16777216;
            long eta_init = (long) d_temp;
            mw_replicated_init(&eta, eta_init);
            printf("initial step size: %lf\n", scaled_float);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--initial-step-decay")){
            sscanf(argv[i+1],"%lf", &scaled_float);
            double d_temp = scaled_float * 16777216;
            long gamma_init = (long) d_temp;
            mw_replicated_init(&gamma, gamma_init);
            printf("initial_step_decay = %lf\n", scaled_float);
            fflush(stdout);
            i++;
        }
    }

}

/**
 * populateTrainingData() reads in test sample data for the current data set, in binary format as output by the csv2bin
 * utility. Training data is stored as three arrays: sample_indicies, feature_indicies, values. Training data arrays are
 * allocated using the mw_malloc1dlong intrinsic operation and are therefore stripped across all nodes in the system.
 * NOTE: In this version of Wildebeest each node gets a complete copy of the training data, this is not necessary but
 * was done for simplicity.
 */
void populateTrainingData() {
    printf("inside populate_data()\n");
    fflush(stdout);

    long i,
            sample = -1,
            feature,
            fixed_value,
            class;
    long j = 0;
    long current_sample = -1;
    long sample_count = -1;
    training_sample_indicies[0] = 0;
    MIGRATE(&model_vector[0]);

    train_data = NULL;
    train_data = fopen(train_data_path, "rb");
    if (train_data == NULL) {
        printf("Failed to open training feature file.\n");
        exit(1);
    }

    long non_zeros = total_train_points - train_sample_count;
    long points;
    long *binBuffer;
    long bytesRead;


    points = non_zeros * 4;
    printf("points = %ld\n", points);
    fflush(stdout);

    binBuffer = (long *) malloc(points * sizeof(long));
    bytesRead = fread(binBuffer, sizeof(long), points, train_data);

    if (bytesRead != (points)) {
        printf("*** Feature File Read Failure ***\n");
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
            training_feature_indicies[j] = 0;
            training_values[j] = 1;
            deg_reciprocal[0]++;
            j++;
            training_sample_indicies[sample_count] = j;
            training_feature_indicies[j] = feature;
            training_values[j] = fixed_value;
            training_classifications[sample_count] = class;
            deg_reciprocal[feature]++;
            current_sample = sample;
        } else {
            training_feature_indicies[j] = feature;
            training_values[j] = fixed_value;
            deg_reciprocal[feature]++;
        }
        j++;
    }
    training_sample_indicies[sample_count + 1] = j; // add sample id end ptr

    fclose(train_data);
    free(binBuffer);

    double d_temp;
    long l_temp;
    for (long i = 0; i <= featureSetSize; i++) {
        d_temp = 1.0;
        d_temp /= (double) deg_reciprocal[i];
        d_temp *= 16777216;
        l_temp = (long) d_temp;
        deg_reciprocal[i] = l_temp;
    }
}


void init() {
    long *lPtr;

    /** Allocate stripped arrays for model vectors and training sample data */
    lPtr = (long *) mw_malloc1dlong(featureSetSize);
    mw_replicated_init((long *) &model_vector, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(featureSetSize);
    mw_replicated_init((long *) &deg_reciprocal, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(train_sample_count + 1);
    mw_replicated_init((long *) &training_sample_indicies, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(total_train_points);
    mw_replicated_init((long *) &training_feature_indicies, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(total_train_points);
    mw_replicated_init((long *) &training_values, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(train_sample_count);
    mw_replicated_init((long *) &training_classifications, (long) lPtr);

    /** Allocate stripped arrays for storing test data */
    lPtr = (long *) mw_malloc1dlong(test_sample_count + 1);
    mw_replicated_init((long *) &test_sample_indicies, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(total_test_points);
    mw_replicated_init((long *) &test_feature_indicies, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(total_test_points);
    mw_replicated_init((long *) &test_values, (long) lPtr);
    lPtr = (long *) mw_malloc1dlong(test_sample_count);
    mw_replicated_init((long *) &test_classifications, (long) lPtr);

    printf("--- Memmory Allocation Complete ---\n");
	fflush(stdout);

	/** Zero-out allocated memory regions prior to running. This will be replaced with memset or its equivalent in
	 * the near future
	 * */
    for (long i = 0; i < featureSetSize; i++) {
        model_vector[i] = 0;
        deg_reciprocal[i] = 0;
    }
    for (long i = 0; i < train_sample_count; i++) {
        training_sample_indicies[i] = 0;
        training_classifications[i] = 0;
    }
    training_sample_indicies[train_sample_count] = 0;
    for (long i = 0; i < total_train_points; i++) {
        training_feature_indicies[i] = 0;
        training_values[i] = 0;
    }

    MIGRATE(&model_vector[0]); /** return to node 0 in preparation for training/testing data placement */
	populateTrainingData(); /** read in training data */
	populateTestData(); /** read in test data */
}
