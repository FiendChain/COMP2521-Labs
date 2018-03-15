// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2014, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DLList.h"

// data structures representing DLList

typedef struct DLListNode {
	char   *value;  // value of this list item (string)
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;

// function prototypes
// free a node
void freeDLListNode(DLListNode *);

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = strdup(it);
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList newDLList()
{
	struct DLListRep *L;

	L = malloc(sizeof (struct DLListRep));
	assert (L != NULL);
	L->nitems = 0;
	L->first = NULL;
	L->last = NULL;
	L->curr = NULL;
	return L;
}

// free up all space associated with list
void freeDLList(DLList L)
{
	assert(L != NULL);
	DLListNode *curr, *prev;
	curr = L->first;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev->value);
		free(prev);
	}
	free(L);
}

// trim off \n from strings (private function)
// this is needed for getDLList() because of fgets()
// alternatively, we could use the evil gets() function
static char *trim(char *s)
{
	int end = strlen(s)-1;
	if (s[end] == '\n') s[end] = '\0';
	return s;
}

// create an DLList by reading items from a file
// assume that the file is open for reading
// assume one item per line, line < 999 chars
DLList getDLList(FILE *in)
{
	DLList L;
	DLListNode *new;
	char line[1000];

	L = newDLList();
	while (fgets(line,1000,in) != NULL) {
		char *value = strdup(trim(line));
		new = newDLListNode(value);
		if (L->last == NULL) {
			L->first = L->last = new;
		}
		else {
			L->last->next = new;
			new->prev = L->last;
			L->last = new;
		}
		L->nitems++;
	}	
	L->curr = L->first;
	return L;
}

// display list to file, one item per line
// assumes that the file is open for writing
void putDLList(FILE *out, DLList L)
{
	assert(out != NULL); assert(L != NULL);
	DLListNode *curr;
	for (curr = L->first; curr != NULL; curr = curr->next)
		fprintf(out,"%s\n",curr->value);
}

// check sanity of a DLList (for testing)
int validDLList(DLList L)
{
	if (L == NULL) {
		fprintf(stderr,"DLList is null\n");
		return 0;
	}
	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL || L->curr != NULL) {
			fprintf(stderr,"Non-null pointers in empty list\n");
			return 0;
		}
	}
	else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL || L->curr == NULL) {
			fprintf(stderr,"Null pointers in non-empty list\n");
			return 0;
		}
	}
	int count;
	DLListNode *curr;
	// check scanning forward through list
	count = 0;
	for (curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf(stderr, "Invalid forward link into node (%s)\n",curr->value);
			return 0;
		}
		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf(stderr, "Invalid backward link into node (%s)\n",curr->value);
			return 0;
		}
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// check scanning backward through list
	count = 0;
	for (curr = L->last; curr != NULL; curr = curr->prev) {
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Backward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// nothing went wrong => must be ok
	return 1;
}

// return item at current position
char *DLListCurrent(DLList L)
{
	assert(L != NULL); assert(L->curr != NULL);
	return L->curr->value;
}

// move current position (+ve forward, -ve backward)
// return 1 if reach end of list during move
// if current is currently null, return 1
int DLListMove(DLList L, int n)
{
	assert(L != NULL);
	if (L->curr == NULL)
		return 1;
	else if (n > 0) {
		while (n > 0 && L->curr->next != NULL) {
			L->curr = L->curr->next;
			n--;
		}
	}
	else if (n < 0) {
		while (n < 0 && L->curr->prev != NULL) {
			L->curr = L->curr->prev;
			n++;
		}
	}
	return (L->curr == L->first || L->curr == L->last) ? 1 : 0;
}

// move to specified position in list
// i'th node, assuming first node has i==1
int DLListMoveTo(DLList L, int i)
{
	assert(L != NULL); assert(i > 0);
	L->curr = L->first;
	return DLListMove(L, i-1);
}

// insert an item before current item
// new item becomes current item
void DLListBefore(DLList L, char *it)
{
	assert(L != NULL); 
	L->nitems++;
	// COMPLETE THIS FUNCTION
	DLListNode *curr = L->curr;
	DLListNode *new = newDLListNode(it);
	// empty
	if(curr == NULL) {
		L->last = L->first = L->curr = new;
		new->next = new->prev = NULL;
		return;
	}
	// not empty
	if(curr->prev != NULL)	// if there is item before curr, link to new
		curr->prev->next = new;
	new->prev = curr->prev;
	new->next = curr;
	curr->prev = new;
	L->curr = new;
	// check if first
	if(new->prev == NULL)
		L->first = new;
}

// insert an item after current item
// new item becomes current item
void DLListAfter(DLList L, char *it)
{
	assert(L != NULL); 
	L->nitems++;
	// COMPLETE THIS FUNCTION
	DLListNode *curr = L->curr;
	DLListNode *new = newDLListNode(it);
	// empty
	if(curr == NULL) {
		L->last = L->first = L->curr = new;
		new->next = new->prev = NULL;
		return;
	}
	// not empty
	if(curr->next != NULL)
		curr->next->prev = new;
	new->next = curr->next;
	new->prev = curr;
	curr->next = new;
	L->curr = new;
	// check if first
	if(new->next == NULL)
		L->last = new;
}

// delete current item
// new item becomes item following current
// if current was last, current becomes new last
// if current was only item, current becomes null
void DLListDelete(DLList L)
{
	assert (L != NULL);
	if(L->nitems == 0) return;
	// COMPLETE THIS FUNCTION
	DLListNode *curr = L->curr;
	L->nitems--;
	// last item or only item
	if(curr->next == NULL) {
		if(curr->prev != NULL) {	// if not only item
			L->curr = L->last = curr->prev;
			curr->prev->next = curr->next;
			freeDLListNode(curr);
			return;
		} else {					// if only item
			L->first = L->last = L->curr = NULL;
			freeDLListNode(curr);
			return;
		}
	}
	// >1 list
	L->curr = curr->next;
	if(curr->prev != NULL)	// if not first item, link (curr->prev) <==> (curr->next)
		curr->prev->next = curr->next;
	else					// if first item, make next item first
		L->first = curr->next;
	curr->next->prev = curr->prev;
	freeDLListNode(curr);
}

// return number of elements in a list
int DLListLength(DLList L)
{
	return (L->nitems);
}

// is the list empty?
int DLListIsEmpty(DLList L)
{
	return (L->nitems == 0);
}

// free a node
void freeDLListNode(DLListNode *node)
{
	if(node == NULL) return;
	free(node->value);
	free(node);
}

// TESTING
void debugList(FILE *fp, DLList L)
{
	assert(validDLList(L));
	DLListNode *curr = L->first;
	while(curr != NULL) {
		if(curr == L->first) fprintf(fp, "first: ");
		if(curr == L->curr)  fprintf(fp, "curr: ");
		if(curr == L->last)	 fprintf(fp, "last: ");
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
	int expectedSize = L->nitems-1;
	DLListNode *expectedCurr = NULL;
	if(L->curr->next != NULL)
		expectedCurr = L->curr->next;
	else
		expectedCurr = L->curr->prev;
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


