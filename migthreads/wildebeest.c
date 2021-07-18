/**
 * Wildebeest: Wildebeest is a shared memory stochastic gradient descent on support vector machines (SGD SVM) for the
 *             purpose of binary classification. It was designed for the Lucata migrating thread architecture. This
 *             code can be used to evaluate training data in originally in the LIBSVM format on a multi-node both the
 *             Chick and Pathfinder-S hardware.
 *
 * Author: Brian A. Page - University of Notre Dame
*/

#include "emu.h" /** Hardware specific headers including memory allocation and remote atomic intrinsics*/
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"
#include "wildebeest.h"
#include "sgd.h"
#include "accuracy.h"

int main(int argc, char **argv) {
    parse_args(argc, argv); /** Get command line parameters */
    mw_replicated_init(&sample_start, 0);
    mw_replicated_init(&sample_end, 0);
    init(); /** Allocate and zero-out associated memory regions */

    /**
     * Return thread to node 0 after traversing the system during memory allocation and data placement. This is
     * necessary since the next phase is to spawn threads in preparation for training, and some thread spawn patterns
     * require being on node 0 to start.
     */
    MIGRATE(&model_vector[0]);
    double accuracy = 0.0;
    long scalar = eta;
    long gamma_temp = gamma;

   /** Spawn threads on each node, having them all call train(). Number of epochs necessary to achieve a desired
    * accuracy varies by dataset and its associated characteristics */
   for (long epoch = 1; epoch <= total_epochs; epoch++) {
       if (epoch > 1) {
                scalar *= gamma_temp;
                scalar >>= 24;
       }
       /** At the start of each epoch, spawn threads on each node and begin training */
       for (long k = 0; k < threads; k++) {
           cilk_migrate_hint(&training_sample_indicies[k]);
           cilk_spawn train(k, scalar, threads);
       }
       cilk_sync;

       accuracy = getModelAccuracy(); /** Evaluate the accuracy of the latest model against the test data */
       printf("Epoch %ld: %lf\n", epoch, accuracy);
       fflush(stdout);
   }

	return 0;
}