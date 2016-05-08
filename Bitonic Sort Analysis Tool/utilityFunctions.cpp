#include "utilityFunctions.h"

void createUnsortedArray(int* elements, int arrayLength){

	//Get size and cuda dimentions from user input

	for (int i = 0; i < arrayLength; ++i){
		elements[i] = rand() % randMax - rand() % 5;
	}

}

bool isSorted(int *elements, int arrayLength){

	bool sorted = true;
	for (int i = 0; i < (arrayLength - 1); ++i){
		if (elements[i] > elements[i + 1]){
			sorted = false;
		}
	}
	return sorted;
}


double getElapsedTime(clock_t start, clock_t stop)
{
	double elapsed = ((double)(stop - start)) / CLOCKS_PER_SEC;
	printf("Elapsed time: %.3fs\n", elapsed);

	return elapsed;
}

int random_int()
{
	return (int)rand() / (int)2048;
}

//Here we'll have three parallel arrays, 
//read in a text file, break it down into line and read those lines into the arrays
BatchArrayValues readBatchTextFiles(char* textFileString){

	int columnPosition = 0;

	int elementCounter = 0;
	int threadCounter = 0;
	int blockCounter = 0;

	//This is the amount of sorts that will be carried out for available inputs in the text file
	int amountOfSorts = 0;

	BatchArrayValues batchArrayValues;

	batchArrayValues.blocks;
	batchArrayValues.threads;
	batchArrayValues.elements;
	char fileName[100] = "";

	strcat(fileName, textFileString);
	
	FILE* file = fopen(fileName, "r");
	char line[1000000];
	char* amountString = "Amount";

	while (fgets(line, sizeof(line), file)) {

		columnPosition = 0;
		char * pch;
		pch = strtok(line, " ");

		while (pch != NULL){

			//If we find amount move over to the int value
			if (strcmp(pch, amountString) == 0){

				pch = strtok(NULL, " ");

				amountOfSorts = atoi(pch);
				batchArrayValues.amountOfSorts = atoi(pch);

				//give every input array enough memeory to hold required inputs 
				batchArrayValues.blocks = (int*)malloc(amountOfSorts*sizeof(int));
				batchArrayValues.elements = (int*)malloc(amountOfSorts*sizeof(int));
				batchArrayValues.threads = (int*)malloc(amountOfSorts*sizeof(int));
				pch = strtok(NULL, " ");

			}
			else{

				if (columnPosition == 0){
					batchArrayValues.elements[elementCounter] = atoi(pch);
					elementCounter++;
				}
				else if (columnPosition == 1){
					batchArrayValues.threads[threadCounter] = atoi(pch);
					threadCounter++;
				}
				else if (columnPosition == 2){
					batchArrayValues.blocks[blockCounter] = atoi(pch);
					blockCounter++;
				}

				pch = strtok(NULL, " ");

				//This symbolises moving along the columns
				columnPosition++;
			}
		}
	}

	fclose(file);

	return batchArrayValues;
}