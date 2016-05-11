
#include "threadPerElementKernel.h"
/*
We get our indexes to swap buy xoring our thread index with the step. This essentially wraps the thread Id round the step value meaning
the only values porduced that are bigger than the Thread Id will be those within the requied step length
*/
__global__ void threadPerElementkernel(int *dev_values, int step, int phaseLength)
{
	unsigned int firstIndex, XoredSecondIndex;
	//Set it to the thread Id
	firstIndex = threadIdx.x + blockDim.x * blockIdx.x;

	XoredSecondIndex = firstIndex ^ step;

	//Threads i corrasponding to the desired bitonic element will be used for the swap
	if ((XoredSecondIndex)>firstIndex) {

		if ((firstIndex&phaseLength) == 0) {
			if (dev_values[firstIndex]>dev_values[XoredSecondIndex]) {

				int temp = dev_values[firstIndex];
				dev_values[firstIndex] = dev_values[XoredSecondIndex];
				dev_values[XoredSecondIndex] = temp;
			}
		}
		if ((firstIndex&phaseLength) != 0) {

			if (dev_values[firstIndex]<dev_values[XoredSecondIndex]) {
				int temp = dev_values[firstIndex];
				dev_values[firstIndex] = dev_values[XoredSecondIndex];
				dev_values[XoredSecondIndex] = temp;
			}
		}
	}

}

/*
Main function call. Created array and calls stepskernel based of the size of the bitonic sequences and step.
*/
void threadPerElementBitonicSortSetup(int *values, int elementsToSort, int deviceBlocks, int threadsPerBlock)
{
	int *dev_values;
	size_t size = elementsToSort* sizeof(int);

	//Allocate memory on the device then copy our host array to device pointer
	cudaMalloc((void**)&dev_values, size);
	cudaMemcpy(dev_values, values, size, cudaMemcpyHostToDevice);

	dim3 blocks(deviceBlocks, 1);
	dim3 threads(threadsPerBlock, 1);

	int step, phaseLength;

	for (phaseLength = 2; phaseLength <= elementsToSort; phaseLength <<= 1) {


		for (step = phaseLength >> 1; step>0; step = step >> 1) {
			threadPerElementkernel << <blocks, threads >> >(dev_values, step, phaseLength);
		}
	}

	cudaMemcpy(values, dev_values, size, cudaMemcpyDeviceToHost);
	cudaFree(dev_values);
}






