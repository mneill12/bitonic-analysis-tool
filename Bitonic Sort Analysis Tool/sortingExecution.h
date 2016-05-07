#include "writeToCSVFile.h"
#include "utilityFunctions.h"

#include "threadPerElementKernel.h"
#include "partitioned2dArrayKernel.cuh"
#include "allStepsInParallelKernel.cuh"


void preExecution();
void threadPerElement();
void twoDimensionArrayPartition();
void stepsInKenralSort();
void runBatchOfSorts();