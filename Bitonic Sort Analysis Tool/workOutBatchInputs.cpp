#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int maxThreadsPerBlock = 1024;
int maxThreadsForCard = 4096;
int twoToThePow32 = 29;

void getSuggestedThreadCounts(int elementcount){

	//Get number of possible combinations which is the exponent of the element count 
	double combinationsCount = log2(double(elementcount));

	//Create struct holding pointer arrays of possible thread and block counts
	int maxiumBlockSize = elementcount / 2;

	int threadsPerBlock = 2;

	int blockSize = 1;

	//Itterate over all possible block sizes that are powers of two, get thread count to make up block size and print to pointer array
	while (threadsPerBlock <= elementcount){


		blockSize = elementcount / threadsPerBlock;

		//Ensure that the threads and blocks fit within the paramaters of the nvida gtx740m
		if (threadsPerBlock <= maxThreadsPerBlock) {
			printf("%d	%d	%d \n", elementcount, threadsPerBlock, blockSize);
		}

		threadsPerBlock *= 2;
	}


}

void runBatchWorkout(){

	int powerOfTwo = 1;

	for (int i = 1; i <= twoToThePow32; i++){

		getSuggestedThreadCounts(powerOfTwo);

		powerOfTwo = powerOfTwo * 2;
	}
}