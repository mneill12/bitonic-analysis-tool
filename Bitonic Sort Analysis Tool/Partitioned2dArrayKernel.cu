#include "partitioned2dArrayKernel.cuh"

arrays splitArray(int* origionalElements, arrays evenOddArrays, int elementCount){

	int *evenCountPtr = evenOddArrays.evenArray;
	int *oddCountPtr = evenOddArrays.oddArray;
	int count = 0;
	for (int i = 0; i < elementCount; i++){

		if (i % 2 == 0){

			memcpy(evenCountPtr, origionalElements, sizeof(int));

			evenCountPtr++;
		}
		else{
			memcpy(oddCountPtr, origionalElements, sizeof(int));

			oddCountPtr++;
		}
		origionalElements++;
	}

	return evenOddArrays;
}

//Merger even and odd arrays into origional arrays
void mergeArrays(arrays evenOddArrays, int* origionalElemens, int elementCount){

	int *evenCountPtr = evenOddArrays.evenArray;
	int *oddCountPtr = evenOddArrays.oddArray;
	int count = 0;

	for (int i = 0; i < elementCount; i++){

		if (i % 2 == 0){

			//Destination, Source, size
			memcpy(origionalElemens, evenCountPtr, sizeof(int));
			evenCountPtr++;
		}
		else{
			memcpy(origionalElemens, oddCountPtr, sizeof(int));
			oddCountPtr++;
		}


		origionalElemens++;
	}

}

bool checkEvenOddArrays(arrays evenOddArrays, int* origionaArray, int elementCount){

	int evenCount = 0;
	int oddCount = 0;

	bool elementSplitCorrectly = true;

	for (int i = 0; i < elementCount; i++){

		if (i % 2 == 0){

			if (origionaArray[i] != evenOddArrays.evenArray[evenCount]){

				elementSplitCorrectly = false;
			}
			evenCount++;
		}

		else{

			if (origionaArray[i] != evenOddArrays.oddArray[oddCount]){

				elementSplitCorrectly = false;
			}
			oddCount++;
		}
	}

	return elementSplitCorrectly;
}

__global__ void bitonicSortAllOtherStepsSingleThreaded(int *deviceElements, int step, int phase, int compExchSize, int fullArraySize)
{
	unsigned int  halfstep, halfphase, secondIndex;

	int firstIndex = threadIdx.x + blockDim.x * blockIdx.x;

	for (int count = 0; count < fullArraySize / 2; count++){
		halfstep = step / 2;
		halfphase = phase / 2;
		secondIndex = firstIndex^halfstep;

		if ((secondIndex) > firstIndex) {
			if ((firstIndex&halfphase) == 0) {

				if (deviceElements[firstIndex] > deviceElements[secondIndex]) {
					int temp = deviceElements[firstIndex];
					deviceElements[firstIndex] = deviceElements[secondIndex];
					deviceElements[secondIndex] = temp;
				}
			}
			if ((firstIndex&halfphase) != 0) {

				if (deviceElements[firstIndex] < deviceElements[secondIndex]) {
					int temp = deviceElements[firstIndex];
					deviceElements[firstIndex] = deviceElements[secondIndex];
					deviceElements[secondIndex] = temp;
				}
			}
		}
		firstIndex++;
	}
}

/*	As we're complying with the origional model but with arrays reduced in size by two
here we'll just divide firstIndex and secondIndex by two to get our required values.
We're also going to go though all the compaire/exchange operations that would normally be carried out in this step.
*/
__global__ void bitonicSortAllOtherSteps(int *deviceElements, int step, int phase, int compExchSize, int fullArraySize)
{
	unsigned int  halfstep, halfphase, secondIndex;

	int firstIndex = threadIdx.x + blockDim.x * blockIdx.x;

	halfstep = step / 2;
	halfphase = phase / 2;
	secondIndex = firstIndex^halfstep;

	if ((secondIndex) > firstIndex) {
		if ((firstIndex&halfphase) == 0) {
			if (deviceElements[firstIndex] > deviceElements[secondIndex]) {
				int temp = deviceElements[firstIndex];
				deviceElements[firstIndex] = deviceElements[secondIndex];
				deviceElements[secondIndex] = temp;
			}
		}
		if ((firstIndex&halfphase) != 0) {

			if (deviceElements[firstIndex] < deviceElements[secondIndex]) {
				int temp = deviceElements[firstIndex];
				deviceElements[firstIndex] = deviceElements[secondIndex];
				deviceElements[secondIndex] = temp;
			}
		}

	}
}

__global__ void bitonicSortFirstStep(int *deviceElements, int step, int phase)
{
	unsigned int firstIndex, secondIndex;
	firstIndex = threadIdx.x + blockDim.x * blockIdx.x;

	secondIndex = firstIndex^step;

	if ((secondIndex)>firstIndex) {
		if ((firstIndex& phase) == 0) {

			if (deviceElements[firstIndex]>deviceElements[secondIndex]) {
				int temp = deviceElements[firstIndex];
				deviceElements[firstIndex] = deviceElements[secondIndex];
				deviceElements[secondIndex] = temp;
			}
		}
		if ((firstIndex&phase) != 0) {

			if (deviceElements[firstIndex]<deviceElements[secondIndex]) {

				int temp = deviceElements[firstIndex];
				deviceElements[firstIndex] = deviceElements[secondIndex];
				deviceElements[secondIndex] = temp;
			}
		}
	}
}

void partitioned2dArrayBitonicSort(int *values, int elementCount, int blockCount, int stepOneThreadCount, int allOtherStepsThreadCount)
{
	int *deviceElements;
	int *deviceEvenArray;
	int *deviceOddArray;
	size_t size = elementCount * sizeof(int);

	size_t evenOddSize = elementCount / 2 * sizeof(int);

	arrays evenOddArrays;


	//Allocate half of element size to each of the odd and even arrays
	evenOddArrays.evenArray = (int*)malloc((elementCount)* sizeof(int));
	evenOddArrays.oddArray = (int*)malloc((elementCount)* sizeof(int));

	cudaMalloc((void**)&deviceElements, size);
	cudaMalloc((void**)&deviceEvenArray, evenOddSize);
	cudaMalloc((void**)&deviceOddArray, evenOddSize);

	dim3 blocks(blockCount, 1);
	dim3 stepOneThreads(stepOneThreadCount, 1);

	dim3 allOtherStepThreads(allOtherStepsThreadCount, 1);

	int compExchCount = (elementCount / 4);

	int step, phase;

	for (phase = 2; phase <= elementCount; phase <<= 1) {
		evenOddArrays = splitArray(values, evenOddArrays, elementCount);

		cudaMemcpy(deviceEvenArray, evenOddArrays.evenArray, evenOddSize, cudaMemcpyHostToDevice);
		cudaMemcpy(deviceOddArray, evenOddArrays.oddArray, evenOddSize, cudaMemcpyHostToDevice);

		for (step = phase >> 1; step > 0; step = step >> 1) {

			if (step != 1){

				//Even and odd arrays to kernals These two calls to the kernal can run in parallel as there is no cudaSyn device calls between them	
				bitonicSortAllOtherSteps << <blocks, allOtherStepThreads >> >(deviceEvenArray, step, phase, compExchCount, elementCount);
				bitonicSortAllOtherSteps << <blocks, allOtherStepThreads >> >(deviceOddArray, step, phase, compExchCount, elementCount);

			}

			//The last step, so copy back the sorted even odd arrays, merge them into the origional element array copy that to memory then sort it 
			else{

				cudaMemcpy(evenOddArrays.evenArray, deviceEvenArray, evenOddSize, cudaMemcpyDeviceToHost);
				cudaMemcpy(evenOddArrays.oddArray, deviceOddArray, evenOddSize, cudaMemcpyDeviceToHost);

				mergeArrays(evenOddArrays, values, elementCount);

				cudaMemcpy(deviceElements, values, size, cudaMemcpyHostToDevice);
				bitonicSortFirstStep << <blocks, stepOneThreads >> >(deviceElements, step, phase);

				cudaMemcpy(values, deviceElements, size, cudaMemcpyDeviceToHost);

			}
		}
	}

	cudaFree(deviceElements);
	cudaFree(deviceEvenArray);
	cudaFree(deviceOddArray);

}