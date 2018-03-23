// TESTING functions
#include <stdlib.h>
#include "DLList.h"

// print list
void debugList(FILE *, DLList);
// move curr pointer to index
void cycleDLList(DLList, int);
// unit tests
int test_DLListBefore(DLList, char *);
int test_DLListAfter(DLList, char *);
int test_DLLDelete(DLList);
// test different files
void debugTestCases(char *);
void debugTestCase(DLList);
void assert_debug(int, DLList);