//
// Created by bpage1 on 4/3/20.
//
// 2^22 = 4194304
// 2^24 = 16777216
//

#include "init.h"
#include "accuracy.h"

void parse_args(int argc, char * argv[]) {
    long num_arg;
    long i;
    double scaled_float;
    train_data_path = NULL;
    test_feature_path = NULL;
    compute_accuracy = 0;
    nodelet_count_start = 1;
    thread_start = 1;
    thread_end = 1;
    nodelet_count_end = 1;
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
            double d_temp = scaled_float * 16777216;
            long eta_init = (long) d_temp;
            mw_replicated_init(&eta, eta_init);
            //initial_step_size = scaled_float;

            printf("initial step size: %lf\n", scaled_float);
            fflush(stdout);
            i++;
        } else if (!strcmp(argv[i],"--initial-step-decay")){
            sscanf(argv[i+1],"%lf", &scaled_float);
            double d_temp = scaled_float * 16777216;
            long gamma_init = (long) d_temp;
            mw_replicated_init(&gamma, gamma_init);
            //initial_step_decay = scaled_float;

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

    long i,
            sample = -1,
            feature,
            fixed_value,
            class;
    long j = 0;
    long current_sample = -1;
    long sample_count = -1;
    training_sample_indicies[0] = 0;
    MIGRATE(&working_vector[0]);

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
            bytesRead = fread(binBuffer, sizeof(long), points, train_data);
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

                //printf("%ld, %ld, %ld, %ld\n", sample, feature, fixed_value, class);
                //fflush(stdout);

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
                    //REMOTE_ADD(&training_values[j], 1);
                    deg_reciprocal[0]++;
                    //REMOTE_ADD(&deg_reciprocal[0], 1);
                    j++;
                    training_sample_indicies[sample_count] = j;
                    //REMOTE_ADD(&training_sample_indicies[sample], 1);
                    training_feature_indicies[j] = feature;
                    //REMOTE_ADD(&training_feature_indicies[j], feature);
                    training_values[j] = fixed_value;
                    //REMOTE_ADD(&training_values[j], fixed_value);
                    training_classifications[sample_count] = class;
                    //REMOTE_ADD(&training_classifications[sample], class);
                    deg_reciprocal[feature]++;
                    //REMOTE_ADD(&deg_reciprocal[feature], 1);
                    current_sample = sample;
                } else {
                    training_feature_indicies[j] = feature;
                    //REMOTE_ADD(&training_feature_indicies[j], feature);
                    training_values[j] = fixed_value;
                    //REMOTE_ADD(&training_values[j], fixed_value);
                    deg_reciprocal[feature]++;
                }
                j++;
            }
        }
        training_sample_indicies[sample_count + 1] = j; // add sample id end ptr
    } else {

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

            //printf("%ld, %ld, %ld, %ld\n", sample, feature, fixed_value, class);
            //fflush(stdout);

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
                //REMOTE_ADD(&training_values[j], 1);
                deg_reciprocal[0]++;
                //REMOTE_ADD(&deg_reciprocal[0], 1);
                j++;
                training_sample_indicies[sample_count] = j;
                //REMOTE_ADD(&training_sample_indicies[sample], 1);
                training_feature_indicies[j] = feature;
                //REMOTE_ADD(&training_feature_indicies[j], feature);
                training_values[j] = fixed_value;
                //REMOTE_ADD(&training_values[j], fixed_value);
                training_classifications[sample_count] = class;
                //REMOTE_ADD(&training_classifications[sample], class);
                deg_reciprocal[feature]++;
                //REMOTE_ADD(&deg_reciprocal[feature], 1);
                current_sample = sample;
            } else {
                training_feature_indicies[j] = feature;
                //REMOTE_ADD(&training_feature_indicies[j], feature);
                training_values[j] = fixed_value;
                //REMOTE_ADD(&training_values[j], fixed_value);
                deg_reciprocal[feature]++;
            }
            j++;
        }
        training_sample_indicies[sample_count + 1] = j; // add sample id end ptr
        //REMOTE_ADD(&training_sample_indicies[sample + 1], j);

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

        printf("populate_data() done\n");
        fflush(stdout);
    }
}

void init_mem(long n) {

}

void reset_nodelet_count(long n, long nodelet_count, long samples_per_nodelet){

}

void init() {
    long *init_temp_Ptr = (long *) mw_malloc1dlong(featureSetSize);
    mw_replicated_init((long *) &working_vector, (long) init_temp_Ptr);

    init_temp_Ptr = (long *) mw_malloc1dlong(featureSetSize);
    mw_replicated_init((long *) &deg_reciprocal, (long) init_temp_Ptr);

    init_temp_Ptr = (long *) mw_malloc1dlong(train_sample_count + 1);
    mw_replicated_init((long *) &training_sample_indicies, (long) init_temp_Ptr);

    init_temp_Ptr = (long *) mw_malloc1dlong(total_train_points);
    mw_replicated_init((long *) &training_feature_indicies, (long) init_temp_Ptr);

    init_temp_Ptr = (long *) mw_malloc1dlong(total_train_points);
    mw_replicated_init((long *) &training_values, (long) init_temp_Ptr);

    init_temp_Ptr = (long *) mw_malloc1dlong(train_sample_count);
    mw_replicated_init((long *) &training_classifications, (long) init_temp_Ptr);

    if (compute_accuracy) {
        init_temp_Ptr = (long *) mw_malloc1dlong(test_sample_count + 1);
        mw_replicated_init((long *) &test_sample_indicies, (long) init_temp_Ptr);

        init_temp_Ptr = (long *) mw_malloc1dlong(total_test_points);
        mw_replicated_init((long *) &test_feature_indicies, (long) init_temp_Ptr);

        init_temp_Ptr = (long *) mw_malloc1dlong(total_test_points);
        mw_replicated_init((long *) &test_values, (long) init_temp_Ptr);

        init_temp_Ptr = (long *) mw_malloc1dlong(test_sample_count);
        mw_replicated_init((long *) &test_classifications, (long) init_temp_Ptr);
    }

    printf("--- Memmory Allocation Complete ---\n");
	fflush(stdout);

    for (long i = 0; i < featureSetSize; i++) {
        working_vector[i] = 0;
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

    MIGRATE(&working_vector[0]);
	printf("--- Memmory Initialization Complete ---\n");
	fflush(stdout);
	populateTrainingData();
    if (compute_accuracy) {
        populateTestData();
    }
	printf("--- Initialization Complete ---\n");
	fflush(stdout);
}
