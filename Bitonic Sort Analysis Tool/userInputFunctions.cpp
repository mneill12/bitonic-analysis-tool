#include "userInputFunctions.h"


bool isPowerOfTwo(int n){

	double logBase2 = log2(double(n));

	if (round(logBase2) == logBase2)
	{
		return true;
	}

	else
	{
		return false;
	}
}

blockAndThreadCounts getElementCounts(){

	int inputCount;

	bool powerOfTwo = false;
	while (!powerOfTwo){

		printf("Enter amount of elements to be sorted: ");
		scanf_s("%d", &inputCount);
		printf("You entered: %d\n", inputCount);

		if (isPowerOfTwo(inputCount))
		{
			powerOfTwo = true;
		}
		else
		{
			printf("Not a power of 2, please re enter\n");
		}
	}

	blockAndThreadCounts suggestedThreadAndBlockCounts = getSuggestedThreadCounts(inputCount);

	return suggestedThreadAndBlockCounts;
}



int getThreadCount(){

	int inputCount;


	printf("Enter amount of threads per block: ");
	scanf_s("%d", &inputCount);
	printf("You entered: %d\n", inputCount);

	

	return inputCount;
}

int getBlockCount(){

	int inputCount;

	bool powerOfTwo = false;
	while (!powerOfTwo){

		printf("Enter amount of blocks: ");
		scanf_s("%d", &inputCount);
		printf("You entered: %d\n", inputCount);

		if (isPowerOfTwo(inputCount))
		{
			powerOfTwo = true;
		}
		else
		{
			printf("/nNot a power of 2, please re enter/n");
		}
	}
	return inputCount;
}

bool runSortAgain(){

	int runAgain;

	printf("Exit execution? 1 to exit any other key to continue ");
	scanf_s("%d", &runAgain);

	if (runAgain == 1){
		printf("Exiting\n");
		return true;
	}

	else{
	
		return false;
	}
}

int getMaxProcessCount(){

	int input;

	printf("Enter maximum amount of times you wish to run the process: ");
	scanf_s("%d", &input);
	printf("You entered: %d\n", input);

	return input;
}

blockAndThreadCounts getSuggestedThreadCounts(int elementcount){

	//Get number of possible combinations which is the exponent of the element count 
	double combinationsCount = log2(double(elementcount));

	//Create struct holding pointer arrays of possible thread and block counts
	blockAndThreadCounts possibleBlockAndThreadCounts;
	possibleBlockAndThreadCounts.elementCount = elementcount;
	possibleBlockAndThreadCounts.combinationsCount = combinationsCount;
	possibleBlockAndThreadCounts.blockCounts = (int*)malloc(combinationsCount*sizeof(int));
	possibleBlockAndThreadCounts.threadCounts = (int*)malloc(combinationsCount*sizeof(int));

	int maxiumBlockSize = elementcount / 2;

	int threadCount = 0;

	int blockSize = 1;

	int positionCounter = 0;
	//Itterate over all possible block sizes that are powers of two, get thread count to make up block size and add to pointer array
	while (blockSize <= maxiumBlockSize){

		threadCount = elementcount / blockSize;
		possibleBlockAndThreadCounts.threadCounts[positionCounter] = threadCount;
		possibleBlockAndThreadCounts.blockCounts[positionCounter] = blockSize;
		blockSize *= 2;
		positionCounter++;
	}

	return possibleBlockAndThreadCounts;
}

int selectTypeOfSort(){
		
	int sortId;
	//print statement 
	printf("Select which Sort you wish to run \n 1 for Multi Step  Bitonic \n 2 for Two dimension Partition Bitonic \n 3 for Thread Per Element Bitonic \n");
	scanf_s("%d", &sortId);

	return sortId;
}
bool selectedRunningType(){

		int individual;

		printf("Do you wish to run individual algorithms or pre set batch? \n1: to select an indvidual algorithm or any other key to run batch of inputs: ");
		scanf_s("%d", &individual);

		if (individual == 1){
			printf("You have selected to run individual algoithms\n");
			return true;
		}

		else{
			printf("You have selected to run all algorithms with conditions stored CSV file \n");
			return false;
		
		}
}
bool runSerialBatch(){

	return false;
}



