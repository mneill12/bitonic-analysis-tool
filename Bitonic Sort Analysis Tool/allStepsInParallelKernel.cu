
#include "allStepsInParallelKernel.cuh"

__global__ void allStepsInParallelBitonicSort(int* deviceElements, int bitonicSequenceSize, int steps){

	//1printf("Kernal Called!!!!");
	/*
	Here we get our first thread var i and j.
	we get j by knowing the size of the subsequence and then halfing it, this gives us the rang that values should be comapired for this step.
	As we go down the steps, we'll be halfing j until step = 1;
	*/

	int firstIndex = threadIdx.x + blockDim.x * blockIdx.x;
	int rangeOfComparison = (bitonicSequenceSize / 2);
	for (int step = steps; step >= 1; step--){

		//This xor op checks that our second value is bigger than our firstIndex value
		if ((firstIndex ^ rangeOfComparison) > firstIndex){

			//assending
			if ((firstIndex / bitonicSequenceSize) % 2 == 0){

				if (deviceElements[firstIndex] > deviceElements[firstIndex ^ rangeOfComparison]) {
					int temp = deviceElements[firstIndex];
					deviceElements[firstIndex] = deviceElements[firstIndex ^ rangeOfComparison];
					deviceElements[firstIndex ^ rangeOfComparison] = temp;
				}

			}
			else{

				if (deviceElements[firstIndex] < deviceElements[firstIndex ^ rangeOfComparison]) {
					int temp = deviceElements[firstIndex];
					deviceElements[firstIndex] = deviceElements[firstIndex ^ rangeOfComparison];
					deviceElements[firstIndex ^ rangeOfComparison] = temp;
				}

			}

		}
		__syncthreads();

		rangeOfComparison = rangeOfComparison / 2;
	}
}
/*
Main function call. Created array and calls stepskernel based of the size of the bitonic sequences and step.
*/
void allStepsInParallelSetup(int* elements, int elementsToSort, int deviceBlocks, int threadsPerBlock){

	int* d_elements;

	//get "phases" so we know how many times we need to send array over to device  
	int phases = int(log2(double(elementsToSort)));

	//General cuda managment here : Allocate on device, array isn't going to change  in size
	cudaMalloc(&d_elements, elementsToSort*sizeof(int));
	cudaMemcpy(d_elements, elements, elementsToSort*sizeof(int), cudaMemcpyHostToDevice);
	dim3 blocks(deviceBlocks, 1);    /* Number of blocks   */
	dim3 threads(threadsPerBlock, 1);  /* Number of threads  */

	for (int currentPhase = 1; currentPhase <= phases; currentPhase++){

		//Get the  size of each sub sequence and the amount of "Steps" in the individual sub sequences 
		int bitonicSequenceSize = int(pow(double(2), double(currentPhase)));

		int steps = int(log2((double)bitonicSequenceSize));

		cudaDeviceSynchronize();
		cudaDeviceSynchronize();
		allStepsInParallelBitonicSort << <blocks, threads >> >(d_elements, bitonicSequenceSize, steps);
		cudaDeviceSynchronize();
	}
	cudaMemcpy(elements, d_elements, elementsToSort*sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(d_elements);
}