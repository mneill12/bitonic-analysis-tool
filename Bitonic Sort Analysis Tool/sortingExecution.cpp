#include "sortingExecution.h"
#include "userInputFunctions.h"

void printArray(int *elements);

int deviceBlocks;
int threadsPerBlock;
int elementsToSort;
int threadCount;

//2dGlobal vars
int allOtherStepsThreadCount;
int allOtherThreadCount;
int stepOneThreadCount;

FILE* batchResults;


//Max times we cann run the process
int executionCount;

void threadPerElement()
{

	executionCount = getMaxProcessCount();
	int fixedExecutionCount = executionCount;

	bool exit = false;

	//Pointers to store our results that we're writing to CSV files, allocate space entered buy the user
	int* threadCounts = (int*)malloc(executionCount*sizeof(int));
	int* allBlocks = (int*)malloc(executionCount*sizeof(int));;
	double* timeResults = (double*)malloc(executionCount*sizeof(double));;
	char* arrayStates = (char*)malloc(executionCount*sizeof(char));

	double time;
	clock_t start, stop;
	//Counter so we can assine values to the array in the execution loop

	while (executionCount != 0){
		//Get thread, blocks and  element count

		//Get total elements and suggested block thread configurations
		blockAndThreadCounts inputCountandSuggestedThreadBlockCount;
		inputCountandSuggestedThreadBlockCount = getElementCounts();
		elementsToSort = inputCountandSuggestedThreadBlockCount.elementCount;

		//wirte possible thread and block configurations to text file
		printf("Writing suggested block thread configuration...");
		writeSuggestedBlockThreadConfigToCsv(inputCountandSuggestedThreadBlockCount.threadCounts,
			inputCountandSuggestedThreadBlockCount.blockCounts,
			inputCountandSuggestedThreadBlockCount.combinationsCount
			);
		printf("Done \n");
		//elementsToSort = inputCountandSuggestedThreadBlockCount.elementCount;
		deviceBlocks = getBlockCount();
		threadsPerBlock = getThreadCount();

		threadCount = threadsPerBlock * deviceBlocks;

		//Malloc array, add values to it and write unsorted array to csv file
		int* values = (int*)malloc(elementsToSort*sizeof(int));
		createUnsortedArray(values, elementsToSort);
		writeBlockElementCsvFile(values, "preSorted", threadCount, deviceBlocks);

		//Do Sort and time it
		start = clock();
		threadPerElementBitonicSortSetup(values, elementsToSort, deviceBlocks, threadsPerBlock);
		stop = clock();

		time = getElapsedTime(start, stop);

		char* arrayState;
		char arrayStateChar;

		if (isSorted(values, elementsToSort)){

			printf("Is Sorted \n");
			arrayState = "sorted";
			arrayStateChar = 's';
		}
		else{

			printf("Not Sorted \n");
			arrayState = "failedToSort";
			arrayStateChar = 'u';
		}

		writeBlockElementCsvFile(values, arrayState, threadCount, deviceBlocks);

		//Allocate results values to pointers 
		*threadCounts = threadCount;
		*allBlocks = deviceBlocks;
		*timeResults = time;
		*arrayStates = arrayStateChar;

		//Increment Result pointers
		threadCounts++;
		allBlocks++;
		timeResults++;
		arrayStates++;

		free(values);

		//Check again for user input

		executionCount--;

		exit = runSortAgain();

		if (exit){ break; }
	}

	printf("Execution ended. Writing results to C:\BitonicSortArrayCSVFiles /n");

	writeSortResultsToCsv(timeResults, "ParallelBitonicSort", arrayStates, threadCounts, allBlocks, fixedExecutionCount - executionCount );

	getchar();
}


void twoDimensionArrayPartition()
{

	executionCount = getMaxProcessCount();
	int fixedExecutionCount = executionCount;

	bool exit = false;

	//Pointers to store our results that we're writing to CSV files, allocate space entered buy the user
	int* threadCounts = (int*)malloc(executionCount*sizeof(int));
	int* allBlocks = (int*)malloc(executionCount*sizeof(int));;
	double* timeResults = (double*)malloc(executionCount*sizeof(double));;
	char* arrayStates = (char*)malloc(executionCount*sizeof(char));

	double time;
	clock_t start, stop;
	//Counter so we can assine values to the array in the execution loop

	while (executionCount != 0){


		//Get thread, blocks and  element count

		//Get total elements and suggested block thread configurations
		blockAndThreadCounts inputCountandSuggestedThreadBlockCount;
		inputCountandSuggestedThreadBlockCount = getElementCounts();
		elementsToSort = inputCountandSuggestedThreadBlockCount.elementCount;

		//wirte possible thread and block configurations to text file
		printf("Writing suggested block thread configuration...");
		writeSuggestedBlockThreadConfigToCsv(inputCountandSuggestedThreadBlockCount.threadCounts,
			inputCountandSuggestedThreadBlockCount.blockCounts,
			inputCountandSuggestedThreadBlockCount.combinationsCount
			);
		printf("Done \n");

		//Get block count and thread count and thena assign half that thread count for all other steps
		deviceBlocks = getBlockCount();
		stepOneThreadCount = getThreadCount();
		allOtherStepsThreadCount = stepOneThreadCount / 2;


		//Malloc array, add values to it and write unsorted array to csv file
		int* values = (int*)malloc(elementsToSort*sizeof(int));
		createUnsortedArray(values, elementsToSort);
		writeBlockElementCsvFile(values, "preSorted", stepOneThreadCount, deviceBlocks);

		//Do Sort and time it
		start = clock();
		partitioned2dArrayBitonicSort(values, elementsToSort, deviceBlocks, stepOneThreadCount, allOtherStepsThreadCount);
		stop = clock();

		time = getElapsedTime(start, stop);

		char* arrayState;
		char arrayStateChar;

		if (isSorted(values, elementsToSort)){

			printf("Is Sorted \n");
			arrayState = "sorted";
			arrayStateChar = 's';
		}
		else{

			printf("Not Sorted \n");
			arrayState = "failedToSort";
			arrayStateChar = 'u';
		}

		writeBlockElementCsvFile(values, arrayState, stepOneThreadCount, deviceBlocks);

		//Allocate results values to pointers 
		*threadCounts = stepOneThreadCount;
		*allBlocks = deviceBlocks;
		*timeResults = time;
		*arrayStates = arrayStateChar;

		//Increment Result pointers
		threadCounts++;
		allBlocks++;
		timeResults++;
		arrayStates++;

		free(values);

		//Check again for user input

		executionCount--;

		exit = runSortAgain();

		if(exit){break;}
	}

	printf("Execution ended. Writing results to C:\BitonicSortArrayCSVFiles /n");

	writeSortResultsToCsv(timeResults, "PartitionedArray2DBitonicSort", arrayStates, threadCounts, allBlocks, fixedExecutionCount - executionCount);

	getchar();

}

void serialSort(){
}


void stepsInKenralSort()
{
	executionCount = getMaxProcessCount();
	int fixedExecutionCount = executionCount;

	bool exit = false;

	//Pointers to store our results that we're writing to CSV files, allocate space entered buy the user
	int* threadCounts = (int*)malloc(executionCount*sizeof(int));
	int* allBlocks = (int*)malloc(executionCount*sizeof(int));;
	double* timeResults = (double*)malloc(executionCount*sizeof(double));;
	char* arrayStates = (char*)malloc(executionCount*sizeof(char));

	double time;
	clock_t start, stop;
	//Counter so we can assine values to the array in the execution loop

	while (executionCount != 0){


		//Get thread, blocks and  element count

		//Get total elements and suggested block thread configurations
		blockAndThreadCounts inputCountandSuggestedThreadBlockCount;
		inputCountandSuggestedThreadBlockCount = getElementCounts();
		elementsToSort = inputCountandSuggestedThreadBlockCount.elementCount;

		//wirte possible thread and block configurations to text file
		printf("Writing suggested block thread configuration...");
		writeSuggestedBlockThreadConfigToCsv(inputCountandSuggestedThreadBlockCount.threadCounts,
			inputCountandSuggestedThreadBlockCount.blockCounts,
			inputCountandSuggestedThreadBlockCount.combinationsCount
			);
		printf("Done \n");
		//elementsToSort = inputCountandSuggestedThreadBlockCount.elementCount;
		deviceBlocks = getBlockCount();
		threadsPerBlock = getThreadCount();

		threadCount = threadsPerBlock * deviceBlocks;

		//Malloc array, add values to it and write unsorted array to csv file
		int* values = (int*)malloc(elementsToSort*sizeof(int));
		createUnsortedArray(values, elementsToSort);
		writeBlockElementCsvFile(values, "preSorted", threadCount, deviceBlocks);

		//Do Sort and time it
		start = clock();
		allStepsInParallelSetup(values, elementsToSort, deviceBlocks, threadsPerBlock);
		stop = clock();

		time = getElapsedTime(start, stop);

		char* arrayState;
		char arrayStateChar;

		if (isSorted(values, elementsToSort)){

			printf("Is Sorted \n");
			arrayState = "sorted";
			arrayStateChar = 's';
		}
		else{

			printf("Not Sorted \n");
			arrayState = "failedToSort";
			arrayStateChar = 'u';
		}

		writeBlockElementCsvFile(values, arrayState, threadCount, deviceBlocks);

		//Allocate results values to pointers 
		*threadCounts = threadCount;
		*allBlocks = deviceBlocks;
		*timeResults = time;
		*arrayStates = arrayStateChar;

		//Increment Result pointers
		threadCounts++;
		allBlocks++;
		timeResults++;
		arrayStates++;

		free(values);

		//Check again for user input

		executionCount--;
		exit = runSortAgain();

		if (exit){ break; }
	}

	printf("Execution ended. Writing results to C:\BitonicSortArrayCSVFiles \n");

	writeSortResultsToCsv(timeResults, "ParallelBitonicSort", arrayStates, threadCounts, allBlocks, fixedExecutionCount - executionCount);

	getchar();
}

int main(void){

	preExecution(); 
	

	//Decided if the user want to run individual algoirthms or all alorithms with predetermined thread block and element inputs.
	bool individualAlgorithms = selectedRunningType();

	//Run the algoithms independently
	if (individualAlgorithms){

		int stepsInKenral = 1;
		int twoDpartition = 2;
		int ThreadPerElement = 3;

		int sortingType = selectTypeOfSort();

		switch (sortingType){

		case 1:
			printf("Runing Steps in Kernel Sort \n");
			stepsInKenralSort();
			break;

		case 2:
			printf("Runing 2 d partition sort \n");
			twoDimensionArrayPartition();
			break;

		case 3:
			printf("Runing thread per element sort \n");
			threadPerElement();
			break;

		}
	}
	else{
		runBatchOfSorts();
	
		}	
}

void runBatchOfSorts(){

	//create and open the batch results csv file globally
	createGlobalBatchResultsFile();

	/*
	For each text file we're going to itterate though them then run the sorts for the paramaters that are in the text files. 
	Then write results to global csv file
	*/
	char* textFiles[3];
	textFiles[0] = "multiStep.txt";
	textFiles[1] = "twoDPartition.txt";
	textFiles[2] = "threadPerElementInputs.txt";
	double time;
	clock_t start, stop;
	printf("Fetching sort paramaters \n");

	int sortNumber = 1;

	for (int sortIndex = 0; sortIndex < 3; sortIndex++){
		//Write sort name to cvs file add space and columns
		fprintf(batchResults, textFiles[sortIndex]);
		fprintf(batchResults, "\n");
		fprintf(batchResults, "State, Blocks, ThreadsPerBlock, Elements, Time,");
		fprintf(batchResults, "\n");

		//Get inputs for specific sort
		BatchArrayValues batchArrayValues = readBatchTextFiles(textFiles[sortIndex]);

		//Running sorts for paramaters in text file
		int sizeOfElementArray = batchArrayValues.amountOfSorts;
	
		for (int i = 0; i < sizeOfElementArray; i++){

			//Allocate memory for the size of each array read in from the text file and create an unsorted array 
			int* values = (int*)malloc(*batchArrayValues.elements*sizeof(int));
			createUnsortedArray(values, *batchArrayValues.elements);

			//Select correct sorting Alorithm for given text file
			switch (sortIndex){

				//TODO Need to overlode these fucntions so we can pass in correct vars
			case 0:
				//printf("Runing Steps in Kernel Sort \n");
				start = clock();
				allStepsInParallelSetup(values, *batchArrayValues.elements, *batchArrayValues.blocks, *batchArrayValues.threads);
				stop = clock();

			case 1:
				//printf("Runing 2 d partition sort \n");
				start = clock();
				partitioned2dArrayBitonicSort(values, *batchArrayValues.elements, *batchArrayValues.blocks, *batchArrayValues.threads, *batchArrayValues.threads/2);
				stop = clock();

			case 2:
				//printf("Runing thread per element sort \n");
				start = clock();
				threadPerElementBitonicSortSetup(values, *batchArrayValues.elements, *batchArrayValues.blocks, *batchArrayValues.threads);
				stop = clock();

			}
		
			time = getElapsedTime(start, stop);

			if (isSorted(values, *batchArrayValues.elements)){
				fprintf(batchResults, "Sorted");
			}
			else{
				fprintf(batchResults, "NotSorted");
			}

			//fprintf(file, "Sorted, Blocks, ThreadsPerBlock, Elements, Time,")
			//wrtie results of sort execution
			fprintf(batchResults, ",%d ", *batchArrayValues.blocks);
			fprintf(batchResults, ",%d ", *batchArrayValues.threads);
			fprintf(batchResults, ",%d", *batchArrayValues.elements);
			fprintf(batchResults, ",%.3f ", time);
		
		

			fprintf(batchResults, "\n");

			//increment pointer arrays holding varaibles
			batchArrayValues.elements++;
			batchArrayValues.blocks++;
			batchArrayValues.threads++;

			//Write all batch results to text file

			free(values);

			printf("Sort Number %d", sortNumber);
			sortNumber++;
		}
	}

}

void preExecution(){

	int values[8];
	values[0] = 10;
	values[1] = 13;
	values[2] = 9;
	values[3] = 18;
	values[4] = 26;
	values[5] = 100;
	values[6] = 3;

	threadPerElementBitonicSortSetup(values, 8, 1, 8);
}

void createGlobalBatchResultsFile(){

	struct tm *tm;
	time_t t;
	char str_time[100];
	char str_date[100];

	t = time(NULL);
	tm = localtime(&t);

	strftime(str_time, sizeof(str_time), "-Time-%H-%M-%S", tm);
	strftime(str_date, sizeof(str_date), "-Date-%d-%m-%Y", tm);

	char* filename = "batchResults";

	char fileDirAndName[120] = "C:/BitonicSortArrayCSVFiles/";
	//Use array state as folder name
	strcat(fileDirAndName, "batchResults");
	strcat(fileDirAndName, "/");
	strcat(fileDirAndName, filename);
	strcat(fileDirAndName, str_date);
	strcat(fileDirAndName, str_time);
	strcat(fileDirAndName, ".csv");

	batchResults = fopen(fileDirAndName, "w");

}
