
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLEN 20            // maximum word length
#define MAXGUESS 10            // maximum number of guesses allowed
#define NUMWORDS 26            // number of words in hangman's dictionary

int getword(int[]);            /* chooses a word from the dictionary for the user to guess -- uses a random number
									   generator to determine which word to choose; a counter array is used to prevent
									   the same word being used twice until all words have been exhausted */

void initguess(char[]);        // initializes array that holds user's correct guesses to '_' character

char getguess(void);        // reads in the letter guessed by the user

int processguess(char[], char[], int, char);    /* compares guessed letter with word to be guessed
												--if guess is correct, letter is placed in user's word array;
												otherwise, wrong guesses are counted */

int checkwin(char[], char[], int);    // checks to see if user has won by matching the two arrays

void showLogo();

void printInstructions();

void drawfig(const int);    // draws hangman figure based on the number of wrong guesses by the user

//****************************************** INT  MAIN  VOID  ***********************************************
int main(void) {

	//hangman dictionary:  List of Flowers a to z
	char* secrets[NUMWORDS] = { "anemone", "bellflower", "camellia", "delphinium", "echinacea", "foxglove", "gladiolus",
		"hyacinth", "iris", "jupitersbeard", "kerria", "lavender", "morningglory", "narcissus",
		"oleander", "poppy", "quince", "rhododendron", "snowdrops", "tulip", "ursinia", "vinca",
		"wisteria", "xeranthemum", "yarrow", "zenobia" };

	char gword[MAXLEN];            // array that holds user's correct guesses

	char guess;                    // letter guessed

	char* challenge;            // current word to be guessed

	int    used[NUMWORDS] = { 0 };    // frequency count of words used so far
	int    wrongs = 0;            // count of wrong guesses
	int wordLen;            // length of current hangman word
	int loop;                // loop counter
	int again = 1;            // flags 1 to play again 0 to quit game
	int won = 0;            // flags 1 if win and 0 if loss

	srand(time(NULL));            //sends a "seed" for random number generation


	while (again)
	{
		initguess(gword);
		challenge = secrets[getword(used)];
		wordLen = strlen(challenge);

		while (wrongs < MAXGUESS && !won)
		{
			drawfig(wrongs);

			for (loop = 0; loop < wordLen; loop++)
				printf("%c ", gword[loop]);

			printf("\n");

			guess = getguess();

			if (!processguess(challenge, gword, wordLen, guess))
				wrongs++;

			if (!won)
				won = checkwin(challenge, gword, wordLen);

			if (won)
			{
				for (loop = 0; loop < wordLen; loop++)
					printf("%c ", gword[loop]);
				printf("\n");
				printf("You win!\n");
				wrongs = 0;
				won = 0;
				break;
			}

			if (wrongs >= MAXGUESS)
			{
				drawfig(wrongs);
				printf("You lost! The word was %s\n", challenge);
				wrongs = 0;
				break;
			}

		}  /* end of inner while loop */

		printf("Do you want to play again? [y/n]: ");
		guess = getchar();
		getchar();
		if (guess == 'n' || guess == 'N')
			again = 0;
	} /* end of outer while loop */

	return 0;
}

int getword(int uct[])
{
	int index, i, sum = 0;

	/* reinitialize used word count to 0 if all words
	have been used -- start recycling */

	for (i = 0; i < NUMWORDS; i++)
		if (uct[i] != 0)
			sum++;
	if (sum == NUMWORDS)
		for (i = 0; i < NUMWORDS; i++)
			uct[i] = 0;

	index = rand() % NUMWORDS;    /* generate a candidate for the challenge */

	while (uct[index])
		index = rand() % NUMWORDS;    /* if this one has been used, get another */

	uct[index]++;        /* mark this index as used */
	return index;
}

void initguess(char g[])
{
	int i;
	for (i = 0; i < MAXLEN; ++i)
		g[i] = '_';
	return;
}

char getguess(void)
{
	char g;
	printf("Your guess: ");
	g = getchar();
	getchar();
	return g;
}

int processguess(char c[], char g[], int l, char gl)
{
	int i, hits = 0;
	for (i = 0; i <= l; i++)
	{
		if (c[i] == gl)
		{
			g[i] = gl;
			hits++;
		}
	}

	return hits;
}

int checkwin(char c[], char g[], int l)
{
	int i, matches = 0;
	for (i = 0; i < l; i++)
		if (c[i] == g[i])
			matches++;
	if (matches == l)
		return 1;
	else
		return 0;
}

void showLogo() {
	printf("**********************************************************\n");
	printf("\n");
	printf("*  H  A  N  G   M  A  N  *\n");
	printf("**********************************************************\n\n");
}

void printInstructions() {
	puts("\t\tWelcome to Hangman!");
	puts("\t\tTry to guess my secret word one letter at a time.");
	puts("\t\tYou can enter both uppercase and lowercase letters.");
	puts("\t\tYou have a total of 10 guesses ... here we go!!\n\n");
}

void drawfig(const int i)
{

	/* clrscr(); */
	system("CLS"); /* clears screen using windows command */

	showLogo();

	printInstructions();

	switch (i)
		/* Each case draws a different part of the hangman stick figure.
		Larger cases intentionally fall through to smaller ones, so
		the drawing is built from the bottom up. */
	{
	case 0:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("____________\n\n");
		break;
	case 1:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 2:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 3:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 4:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |\n");
		printf("  |\n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 5:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |    |\n");
		printf("  |    |\n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 6:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |   \\|\n");
		printf("  |    | \n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 7:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |   \\|/\n");
		printf("  |    | \n");
		printf("  |\n");
		printf("__|_________\n\n");
		break;
	case 8:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |   \\|/\n");
		printf("  |    | \n");
		printf("  |   /\n");
		printf("__|_________\n\n");
		break;
	case 9:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    O \n");
		printf("  |   \\|/\n");
		printf("  |    | \n");
		printf("  |   / \\\n");
		printf("__|_________\n\n");
		break;
	case 10:
		printf("Amount of wrong letters: %d\n\n", i);
		printf("  _______\n");
		printf("  |/   | \n");
		printf("  |    X \n");
		printf("  |   \\|/\n");
		printf("  |    | \n");
		printf("  |   / \\\n");
		printf("__|_________\n\n");
		break;
	}
	printf("\n");
	return;
}
