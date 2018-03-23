// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "test_DLList.h"

// cycle through test cases
# define TOTAL_TEST_CASES 4
char testFiles[TOTAL_TEST_CASES][FILENAME_MAX] = {
	"test-00",
	"test-01",
	"test-02",
	"test-03",
};

int main(int argc, char *argv[])
{
	for(int i = 0; i < TOTAL_TEST_CASES; i++) 
		debugTestCases(testFiles[i]);
	return 0;
}


