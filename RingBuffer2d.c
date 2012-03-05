/*
 
Copyright (c) 2012 R. Brett Jones

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RingBuffer2d.h"

int roundNextPowerof2(int inVal) {
    int pow2 = 1;
    while (inVal > pow2 && pow2 < (1 << 30) - 1) {
        pow2 = pow2 << 1;
    }
    return pow2;  
}

// numRow will be rounded up to a power of 2
RingBuffer2d * rb2dInit(int numCol, int numRow, size_t elemsize) {
	RingBuffer2d *rb2d = malloc(sizeof(RingBuffer2d));
	rb2d->rows = roundNextPowerof2(numRow);
	rb2d->cols = numCol;
	rb2d->eleSize = elemsize;
	rb2d->writeIndex = rb2d->readIndex = 0;

	rb2d->buffer = malloc(rb2d->rows * sizeof(void *));

	int i;
	for (i = 0; i < rb2d->rows; ++i)
	{
		rb2d->buffer[i] = calloc(rb2d->cols, rb2d->eleSize);
	}
	printf("Initialized rb2d. Row:%i Col:%i\n",rb2d->rows,rb2d->cols);

	return rb2d;
}

void rb2dPush(RingBuffer2d *rb2d, void *array) {
	memmove(rb2d->buffer[rb2d->writeIndex++ & rb2d->rows - 1],array,rb2d->cols * rb2d->eleSize);
}

void rb2dPop(RingBuffer2d *rb2d, void *array) {
	memmove(array, rb2d->buffer[rb2d->readIndex++ & rb2d->rows - 1],rb2d->cols * rb2d->eleSize);
}

void rb2dReadCol(RingBuffer2d *rb2d, void *array, int colIndex, int n) {
	int i;
	for (i = 0; i < n; i++) {
		memmove(array + (i * rb2d->eleSize),rb2d->buffer[(rb2d->readIndex - i) & rb2d->rows - 1] + colIndex * rb2d->eleSize,rb2d->eleSize);
	}	
}

// Returns number of read/writeable rows
size_t rb2dWriteCapacity(RingBuffer2d *rb2d) {

}
size_t rb2dReadCapacity(RingBuffer2d *rb2d) {

}

int rb2dFull(RingBuffer2d *rb2d) {

}
int rb2dEmpty(RingBuffer2d *rb2d) {

}

void rb2dPrintInt(RingBuffer2d *rb2d) {
	printf("Printing all data as int.\n");
	int i,j;
	for (i = 0; i < (int)rb2d->rows; i++)
	{
		printf("\n");
		void *bufferStart = rb2d->buffer[i];
		for (j = 0; j < rb2d->cols; j++)
		{
			printf("%i,",*((int *)bufferStart + j));
		}
	}
	printf("End of line\n");
}

void rb2dPrintFloat(RingBuffer2d *rb2d) {
	printf("Printing all data as float\n");
	int i,j;
	for (i = 0; i < rb2d->rows; i++)
	{
		void *bufferStart = rb2d->buffer[i];
		for (j = 0; j < rb2d->cols; j++)
		{
			printf("%f,",*((float *)bufferStart + j));
		}
		printf("End of line\n");
		
	}
}

void printIntArray(int *array, int len) {
	int i;
	for (i = 0; i < len; i++) {
		printf("%i,",array[i]);
	}
	printf("\n"); 
}

void printDetails(RingBuffer2d *rb2d) {
	printf("Rows:%i Cols:%i Read:%i Write:%i\n",rb2d->rows,rb2d->cols,rb2d->readIndex,rb2d->writeIndex); 
}

int main(void) {
    printf("\n::Testing each function::\n");
    RingBuffer2d *rb2d = rb2dInit(10,14,sizeof(int));
    rb2dPrintInt(rb2d);

    int *emptyArr = calloc(rb2d->cols, sizeof(int));
    int *onesArr = calloc(rb2d->cols, sizeof(int));
    //memset(onesArr,1,rb2d->cols * sizeof(int));
    int *arr1 = malloc(rb2d->cols * sizeof(int));
    int *arr2 = malloc(rb2d->cols * sizeof(int));
    int *popArr = calloc(rb2d->cols, sizeof(int));
    int i;
    for (i = 0; i < rb2d->cols; i++)
    {
    	arr1[i] = i;
	arr2[i] = 2 * i;
	onesArr[i] = 1;
    }

    printDetails(rb2d);
//    rb2dPush(rb2d, emptyArr);
    rb2dPush(rb2d, arr1);
    rb2dPush(rb2d, onesArr);
    rb2dPush(rb2d, arr2);
    printDetails(rb2d);
    rb2dPrintInt(rb2d);

    int *colPop = malloc(5 * sizeof(int));
    rb2dReadCol(rb2d,colPop,3,5);
    printIntArray(colPop,5);
   
    printIntArray(popArr,rb2d->cols);
    rb2dPop(rb2d,popArr);

    printDetails(rb2d);

    printIntArray(popArr,rb2d->cols);
    rb2dPop(rb2d,popArr);

    printIntArray(popArr,rb2d->cols);
    printDetails(rb2d);


    rb2dReadCol(rb2d,colPop,3,5);
    printIntArray(colPop,5);
}


