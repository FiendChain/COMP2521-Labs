#include "test_DLList.h"
#include <assert.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// show debug info for a dllist
void debugList(FILE *fp, DLList L)
{
	assert(validDLList(L));
    fprintf(fp, "Total items: %d\n", L->nitems);
    fprintf(fp, "Addr L->first: %p\n", (void *)L->first);
    fprintf(fp, "Addr L->curr: %p\n", (void *)L->curr);
    fprintf(fp, "Addr L->last: %p\n", (void *)L->last);
    if(L->nitems == 0) {
        fprintf(fp, "(empty)\n");
        return;
    }
	DLListNode *curr = L->first;
	while(curr != NULL) {
		if(curr == L->curr)  fprintf(fp, "*");
		fprintf(fp, "%s\n", curr->value);
		curr = curr->next;
	}
}

// cycle to appropriate index
void cycleDLList(DLList L, int index)
{
	DLListNode *curr = L->first;
	for(int i = 0; i < L->nitems && curr != NULL; i++)
	{
		if(index == i || curr->next == NULL) {
			L->curr = curr;
			return;
		}
		curr = curr->next;
	}
}

// UNIT TESTS
int test_DLListBefore(DLList L, char *it)
{
	int returnValue = 1;
	DLListNode *oldcurr = L->curr;
	int oldSize = L->nitems; 
	DLListBefore(L, it);
	assert(validDLList(L));
	if(strcmp(L->curr->value, it) == 0) {
		if(L->curr->next != oldcurr) {
			fprintf(stderr, "Error: DLListBefore() failed to have oldcurr after new curr\n");
			returnValue = 0;
		}
		if(L->nitems != oldSize+1) {
			fprintf(stderr, "Error: DLListBefore() did not increment list size!\n");
			returnValue = 0;
		}
	} else {
		fprintf(stderr, "Error: DLListBefore() failed to insert value to current\n");
		returnValue = 0;
	}
	return returnValue;
}

int test_DLListAfter(DLList L, char *it)
{
	int returnValue = 1;
	DLListNode *oldcurr = L->curr;
	int oldSize = L->nitems; 
	DLListAfter(L, it);
	assert(validDLList(L));
	if(strcmp(L->curr->value, it) == 0) {
		if(L->curr->prev != oldcurr) {
			fprintf(stderr, "Error: DLListAfter() failed to have oldcurr before new curr\n");
			returnValue = 0;
		}
		if(L->nitems != oldSize+1) {
			fprintf(stderr, "Error: DLListAfter() did not increment list size!\n");
			returnValue = 0;
		}
	} else {
		fprintf(stderr, "Error: DLListAfter() failed to insert value to current\n");
		returnValue = 0;
	}
	return returnValue;
}

int test_DLLDelete(DLList L)
{
	int returnValue = 1;
	// predict expected new list
	int expectedSize = (L->nitems) > 0 ? L->nitems-1 : 0;

	DLListNode *expectedCurr = NULL;
    if(L->curr != NULL) {
        if(L->curr->next != NULL)
            expectedCurr = L->curr->next;
        else
            expectedCurr = L->curr->prev;
    }
	DLListDelete(L);
	assert(validDLList(L));
	if(L->curr != expectedCurr) {
		fprintf(stderr, "Error: DLLDelete() did not change current to expected node\n");
		returnValue = 0;
	}
	if(L->nitems != expectedSize) {
		fprintf(stderr, "Error: DLLDelete() did not decrement size of list\n");
		returnValue = 0;
	}

	return returnValue;
}

// given a filename, run all test case
void debugTestCases(char *filename)
{
    assert(filename != NULL);
    FILE *in = fopen(filename, "r");
    if(in == NULL) {
        fprintf(stderr, "Could not read or allocated memory for: %s\n", filename);
        return;
    }
    DLList list = getDLList(in);
    int totalLines = DLListLength(list);
    freeDLList(list);
    for(int curr = 0; curr < totalLines; curr++) {
        printf(">> Testing: %s @ index: %d\n", filename, curr);
        // test without changing current
        DLList myList = getDLList(in);
        printf("Original: \n");
        debugList(stdout,myList);
        printf("\n");
        cycleDLList(myList, curr);
        debugTestCase(myList);
        freeDLList(myList);	
        printf("\n\n");
    }
    fclose(in);
}

// debug a given list with random operations
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

// if assert fails, then print list and exit
void assert_debug(int assert_val, DLList myList)
{
	if(!assert_val) {
		debugList(stdout, myList);
		exit(EXIT_FAILURE);
	}
}