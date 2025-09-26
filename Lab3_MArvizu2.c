/*	ENG175IN Spring2020
	
	Created by Maria Arvizu on 01-28-2021
	Copyright @MArvizu. All rights reserved. 

	This program will calculate temperature of
	coffee using Newtons Law of Temperature.

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>


int main(void){

	double t;
	double temp;

	printf("Newtons Law for Coffee Temperature: \n");
	printf("Enter TIME (min):  \n");
	scanf("%lf", &t);

	temp = 70.0 + 130.0 * exp(-0.049 * t);

	printf("******************************\n\n");
	printf("Coffee Temperature = %.2lf degrees F\n", temp);







		return 0;
	}