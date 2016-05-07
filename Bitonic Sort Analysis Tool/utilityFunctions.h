#include<time.h>
#include<math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 



struct BatchArrayValues{

	int *elements;
	int *threads;
	int *blocks;
};

const int randMax = 10000;

void createUnsortedArray(int* elements, int arrayLength);
bool isSorted(int *elements, int arrayLength);
double getElapsedTime(clock_t start, clock_t stop);
int random_int();
BatchArrayValues readBatchTextFiles(char* textFileString);