#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda_runtime_api.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include <cuda_runtime_api.h>

__global__ void threadPerElementkernel(int *dev_values, int step, int phaseLength);
void threadPerElementBitonicSortSetup(int *values, int elementsToSort, int deviceBlocks, int threadsPerBlock);