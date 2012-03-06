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

/*
  Struct for maintaining a rolling history of arrays. Rows is the history length,
  cols is the length of the array.
*/

typedef struct RingBuffer2d {
    void **buffer;
    volatile unsigned int writeIndex; //row
    volatile unsigned int readIndex; //row
    int rows;
    int cols;
    size_t count;
    size_t eleSize;
} RingBuffer2d;

// Rows will be rounded to next power of 2. Cols has no restrictions
RingBuffer2d * rb2dInit(int numCol, int numRow, size_t elemsize);
void rb2dDestroy(RingBuffer2d *rb2d);

void rb2dPush(RingBuffer2d *rb2d, void *array);

void rb2dPop(RingBuffer2d *rb2d, void *array);

void rb2dReadCol(RingBuffer2d *rb2d, void *array, int colIndex, int n);

// Returns number of read/writeable rows
size_t rb2dWriteCapacity(RingBuffer2d *rb2d);
size_t rb2dReadCapacity(RingBuffer2d *rb2d);

int rb2dFull(RingBuffer2d *rb2d);
int rb2dEmpty(RingBuffer2d *rb2d);

void rb2dPrintInt(RingBuffer2d *rb2d);
void rb2dPrintFloat(RingBuffer2d *rb2d);

int roundNextPowerof2(int inVal);
