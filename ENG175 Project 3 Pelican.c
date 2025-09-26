#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct {
	int customer;
	char type[15];
	int item;
	double netsale;
	char payment[20];
	char gender[8];
	char marital[8];
	int age;
} structPelican;

int main(void){


	//declare variables
	structPelican store[150];
	int i = 0;
	int numRecords = 0;

	//declare read file and output file
	FILE *storeData = fopen("C:\\Users\\ysandoval\Desktop\\PelicanStoresDATA.txt", "r");
	FILE *storeOUT = fopen("C:\\Users\\ysandoval\\Desktop\\PelicanStoresOUT.txt", "w");

	//check if file is properly read
	if (storeData == NULL){
		printf("Error!  Not able to read input file.\n");
	}
	else {
		printf("Read successful!\n");

		//read the records in to the array of structures
		while (fscanf(storeData, "%d", &store[i].customer) != EOF){
			fscanf(storeData, "%s", store[i].type);
			fscanf(storeData, "%d", &store[i].item);
			fscanf(storeData, "%lf", &store[i].netsale);
			fscanf(storeData, "%s", store[i].payment);
			fscanf(storeData, "%s", store[i].gender);
			fscanf(storeData, "%s", store[i].marital);
			fscanf(storeData, "%d", &store[i].age);
			i = i + 1;
		} //end while loop for reading records

		numRecords = i;

		//data calculations





		//print the records in the database
		fprintf(storeOUT, "-------------------------------------------------------------------------\n");
		fprintf(storeOUT, "\tCustomer\tType of Customer\tItems\tNet Sales\tMethod of payment\tGender\tMarital status\tAge\n");
		for (i = 0; i < numRecords; i++){
			fprintf(storeOUT, "\t%d", store[i].customer);
			fprintf(storeOUT, "\t\t%15s", store[i].type);
			fprintf(storeOUT, "\t\t%d", store[i].item);
			fprintf(storeOUT, "\t%.1lf", store[i].netsale);
			fprintf(storeOUT, "\t%22s", store[i].payment);
			fprintf(storeOUT, "\t\t%s", store[i].gender);
			fprintf(storeOUT, "\t%s", store[i].marital);
			fprintf(storeOUT, "\t%10d", store[i].age);
			fprintf(storeOUT, "\n");

		}  //end for loop for printing records

	}  //end else statement

	//must close any files that were opened - a 5 point deduction if not included in any program
	fclose(storeData);
	fclose(storeOUT);

	return 0;
}

