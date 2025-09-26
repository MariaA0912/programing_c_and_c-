//required heading

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void) {

	int i;
	char inputString[100];
	int length;
	int count = 0;

	printf("Counting the number of e and E's in your string.\n");
	printf("\n\tEnter your favorite sports team: ");
	gets(inputString);

	length = strlen(inputString);
	printf("\n\t%d = length of input\n", length);

	//Count the number of e and E's in the string
	for (i = 0; i < length; i++) {

		if ((inputString[i] == 'e') || (inputString[i] == 'E')) {
			count++;      //or count = count + 1;
		}

	} 
	printf("\t%d = number of e or E's in the string\n", count);
	
	return 0;
}