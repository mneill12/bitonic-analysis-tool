#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_runtime_api.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include <cuda_runtime_api.h>

#include<math.h>

__global__ void allStepsInParallelBitonicSort(int* deviceElements, int bitonicSequenceSize, int steps);
void allStepsInParallelSetup(int* elements, int elementsToSort, int deviceBlocks, int threadsPerBlock);
