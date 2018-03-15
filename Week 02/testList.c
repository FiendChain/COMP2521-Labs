// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "DLList.h"

// cycle through test cases
# define TOTAL_TEST_CASES 4
char testFiles[TOTAL_TEST_CASES][FILENAME_MAX] = {
	"test-00",
	"test-01",
	"test-02",
	"test-03",
};

void debugTestCases();
void debugTestCase(DLList);
void assert_debug(int, DLList);

int main(int argc, char *argv[])
{
	debugTestCases();
	return 0;
}

void debugTestCases()
{
	for(int i = 0; i < TOTAL_TEST_CASES; i++) {
		FILE *originalFile = fopen(testFiles[i], "r");
		if(originalFile == NULL) {
			fprintf(stderr, "IOError: Could not open test case: %s\n", testFiles[i]);
			continue;
		}
		DLList originalList = getDLList(originalFile);
		int totalLines = DLListLength(originalList);
		fclose(originalFile);
		freeDLList(originalList);
		for(int currLocation = 0; currLocation < totalLines; currLocation++) {
			printf(">> Testing: %s @ index: %d\n", testFiles[i], currLocation);
			FILE *fp = fopen(testFiles[i], "r");
			assert(fp != NULL);
			// test without changing current
			DLList myList = getDLList(fp);
			cycleDLList(myList, currLocation);
			debugTestCase(myList);
			freeDLList(myList);	
			fclose(fp);
			printf("\n\n");
		}
	}
}

void debugTestCase(DLList L)
{
	int testCase = 0;
	char message[50] = {0};
	srand(time(NULL));
	for(int i = 0; i < 50; i++) {
		testCase = abs(rand()%3);
		snprintf(message, 50, "Message %d", i);
		switch(testCase) {
		case 0:
			printf("> DLListBefore(%s)\n", message);
			assert_debug(test_DLListBefore(L, message), L);	// insert before
			break;
		case 1:
			printf("> DLListAfter(%s)\n", message);
			assert_debug(test_DLListAfter(L, message), L);	// insert after
			break;
		case 2:
			printf("> DLListDelete()\n");
			assert_debug(test_DLLDelete(L), L);
			break;
		default:
			printf("> Warning: Unknown case %d\n", testCase);
			continue;
		}
		debugList(stdout, L);
		printf("\n");
		assert(validDLList(L));
	}
}

void assert_debug(int assert_val, DLList myList)
{
	if(!assert_val) {
		debugList(stdout, myList);
		exit(EXIT_FAILURE);
	}
}
