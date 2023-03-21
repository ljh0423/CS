#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "array.h"

static bool initialized = false;
static int numReads = 0;
static int numWrites = 0;
static int theArray[ArraySize];

static void cleanup() {
    fprintf(stderr, "reads: %d\n", numReads);
    fprintf(stderr, "writes: %d\n", numWrites);
}

static void init() {
    initialized = true;
    atexit(cleanup);
}

void put(int index, int contents) {
    if (!initialized) init();
    numWrites++;
    theArray[index] = contents;
}

int get(int index) {
    if (!initialized) init();
    numReads++;
    return theArray[index];
}

void pp(){
  for(int i=0;i<ArraySize;i++){
    printf("%d, ", theArray[i]);
  }
  printf("\n");
}