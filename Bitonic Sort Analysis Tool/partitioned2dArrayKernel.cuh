#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_runtime_api.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include <cuda_runtime_api.h>


#include "writeToCSVFile.h"


struct arrays
{
	int* evenArray;
	int* oddArray;
};

arrays splitArray(int* origionalElements, arrays evenOddArrays, int elementCount);
void mergeArrays(arrays evenOddArrays, int* origionalElemens, int elementCount);
bool checkEvenOddArrays(arrays evenOddArrays, int* origionaArray, int elementCount);
__global__ void bitonicSortAllOtherStepsSingleThreaded(int *deviceElements, int step, int phase, int compExchSize, int fullArraySize);
__global__ void bitonicSortAllOtherSteps(int *deviceElements, int step, int phase, int compExchSize, int fullArraySize);
__global__ void bitonicSortFirstStep(int *deviceElements, int step, int phase);
void partitioned2dArrayBitonicSort(int *values, int elementCount, int blockCount, int stepOneThreadCount, int allOtherStepsThreadCount);