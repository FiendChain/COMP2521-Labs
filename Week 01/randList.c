// randList.c - generate a list of random integers
// Written by John Shepherd, July 2008
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void genKey(int value);
int checkRepeat();

int main(int argc, char *argv[])
{
	int max, i, k;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s #values type [seed]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	max = atoi(argv[1]);
	if (max < 1) {
		fprintf(stderr, "%s: too few values\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (max > 10000000) {
		fprintf(stderr, "%s: too many values\n", argv[0]);
		exit(EXIT_FAILURE);
	}
    srand(time(NULL)); // really random
    // get types of cases
    char sortType = argv[2][0];
    switch(sortType) {
        case 'A':
            k = 0;
            for (i = 0; i < max; i++) {
                if(checkRepeat()) k++;
                genKey(k);
            }
            break;
        case 'R':
            for (i = 0; i < max; i++) {
                int randVal = rand();
                randVal = randVal >= 0 ? randVal : -randVal;
                genKey(randVal%(1+max));
            }
            break;
        case 'D':
            k = max;
            for (i = max; i >= 0; i--) {
                if(checkRepeat()) k--;
                genKey(k);
            }
            break;
        default:
            printf("Error: %c is not a valid option!\n", sortType);
            return EXIT_FAILURE;
    }
	
    
	return 0;
}

// check if repeat or not
int checkRepeat()
{
    int randVal = rand();
    randVal = randVal >= 0 ? randVal : -randVal;
    int repeat = randVal % 2;
    return repeat;
}

// generate random key
void genKey(int value)
{
    char key[10] = {0};
    for(int i = 0; i < 3; i++)
    {
        int randVal = rand();
        randVal = randVal >= 0 ? randVal : -randVal;
        int charOffset = randVal%26;
        key[i] = 'a'+charOffset;
    }
    printf("%d %s\n",value, key);
}
