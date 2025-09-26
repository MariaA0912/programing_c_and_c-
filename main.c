/*	ENG175IN Spring2021
	
	Created by Maria Arvizu on 04-20-2021

	Copyright @MArvizu. All rights reserved. 

	This program will work with triangles. 

*/

#define	_CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <ctype.h>
#include <math.h>

void pascalTriangle();
double heronsformula(double* ptrArea);
int main(void) {

	char loopAgian;
	int userChoice;
	double area;
	do {
		printf("............... m e n u ...............\n");
		printf("1. Pascal's Triangle\n");
		printf("2. Heron's Formula\n");
		scanf("%d", &userChoice);

		switch (userChoice)
		{
		case 1:
			printf("You chose Pascal's Triangle.....\n");
			pascalTriangle();
			break;
		case 2:
			printf("You chose Heron's Formula.....\n");
			heronsformula(&area);
			printf("\tArea = %.2lf\n", area);
			break;
		default:
			printf("ERROR! You MUST chose a number from 1-3\n");
			break;
		}


		printf("Again? y/n");
		scanf(" %c", &loopAgian);
		loopAgian = tolower(loopAgian);

	} while (loopAgian == 'y');


	return 0;
}
void pascalTriangle() {

	int rows, coef = 1, space, i, j;

	printf("Enter the number of rows: ");
	scanf("%d", &rows);

	for (i = 0;i<rows;i++) {
		for (space = 1;space <= rows - 1;space++) {
			printf("  ");
		}
		for (j = 0;j <= i;j++) {
			if (j == 0 || i == 0)
				coef = 1;
			else
				coef = coef * (i - j + 1) / j;
			printf("%4d", coef);
		}
		
		printf("\n");
	}

	return 0;
	}
double heronsformula(double *ptrArea) {
	double p, a, b, c;//p is half perimeter, side a, side b, side c
	
	printf("\tenter side a: ");
	scanf("%lf", &a);
	printf("\tenter side b: ");
	scanf("%lf", &b);
	printf("\tenter side c: ");
	scanf("%lf", &c);

	p = (a + b + c) / 2.0;

	*ptrArea = sqrt(p * (p - a) * (p - b) * (p - c));



	return;
}