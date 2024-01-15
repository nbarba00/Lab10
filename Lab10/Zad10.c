#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH (50)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)

typedef struct Town {
	char name[MAX_LENGTH];
	int population;
	struct Town* next;
	struct Town* left;
	struct Town* right;
} Town;

typedef struct Country {
	char name[MAX_LENGTH];
	struct Country* next;
	struct Country* left;
	struct Country* right;
	Town* townTreeRoot;
	Town townListHead;
} Country;

Country* readAndFillCountries(Country* countryHeadList, Country* countryHeadTree);
int insertSortedNewCountryList(Country* townHead, Country* newCountry);
Country* insertNewCountryTree(Country* root, Country* newCountry);
Country* createNewCountry(char* countryName);
Town* createNewTown(char* townName, int townPopulation);
int insertSortedNewTownList(Town* townHead, Town* newTown);
Town* insertNewTownTree(Town* root, Town* newTown);
int printCountryList(Country* countryHeadList);
int printCountryTree(Country* countryHeadTree);
int printTownList(Town* townHeadList);
int printTownTree(Town* townHeadTree);

int main() {
	Country countryHeadList = { .name = { 0 }, .next = NULL, .left = NULL, .right = NULL, .townTreeRoot = NULL,
								.townListHead = {.name = { 0 }, .population = 0, .next = NULL, .left = NULL, .right = NULL } };

	Country* countryHeadTree = NULL;

	countryHeadTree = readAndFillCountries(&countryHeadList, countryHeadTree);
	printCountryList(&countryHeadList);
	printf("\n\n\n\n");
	printCountryTree(countryHeadTree);

	return EXIT_SUCCESS;
}

Country* readAndFillCountries(Country* countryHeadList, Country* countryHeadTree) {
	FILE* filePointer = NULL;
	char countryName[MAX_LINE] = { 0 };
	char countryFile[MAX_LINE] = { 0 };
	char nullString[MAX_LINE] = { 0 };
	Country* newCountryList = NULL;
	Country* newCountryTree = NULL;
	Country* countries[] = { NULL, NULL };

	filePointer = fopen("drzave.txt", "r");
	if (!filePointer) {
		printf("Can't open file!\n");
		return FILE_NOT_OPEN;
	}

	while (fscanf(filePointer, " %s %s", countryName, countryFile) == 2) {
		newCountryList = createNewCountry(countryName);
		newCountryTree = createNewCountry(countryName);
		countries[0] = newCountryList;
		countries[1] = newCountryTree;

		// Read city file, create town structures, and populate country structures
		FILE* cityFilePointer = fopen(countryFile, "r");
		if (!cityFilePointer) {
			printf("\033[31mCan't open city file!\033[0m\n");
			return FILE_NOT_OPEN;
		}

		while (fscanf(cityFilePointer, " %s %d", countryName, &townPopulation) == 2) {
			Town* newTownList = createNewTown(townName, townPopulation);
			Town* newTownTree = createNewTown(townName, townPopulation);
			insertSortedNewTownList(&newCountryTree->townListHead, newTownList);
			newCountryList->townTreeRoot = insertNewTownTree(newCountryList->townTreeRoot, newTownTree);
		}

		fclose(cityFilePointer);

		insertSortedNewCountryList(countryHeadList, newCountryList);
		countryHeadTree = insertNewCountryTree(countryHeadTree, newCountryTree);
		strcpy(countryName, nullString);
		strcpy(countryFile, nullString);
	}

	fclose(filePointer);

	return countryHeadTree;
}

Country* createNewCountry(char* countryName) {
	Country* country = (Country*)malloc(sizeof(Country));
	if (!country) {
		printf("Can't allocate memory!\n");
		exit(EXIT_FAILURE);
	}

	strcpy(country->name, countryName);
	country->next = NULL;
	country->left = NULL;
	country->right = NULL;
	country->townTreeRoot = NULL;
	country->townListHead.population = 0;
	country->townListHead.next = NULL;
	country->townListHead.left = NULL;
	country->townListHead.right = NULL;

	return country;
}

Town* createNewTown(char* townName, int townPopulation) {
	Town* town = (Town*)malloc(sizeof(Town));
	if (!town) {
		printf("Can't allocate memory!\n");
		exit(EXIT_FAILURE);
	}

	strcpy(town->name, townName);
	town->population = townPopulation;
	town->next = NULL;
	town->left = NULL;
	town->right = NULL;

	return town;
}

int insertSortedNewTownList(Town* townHead, Town* newTown) {
	Town* townCurrent = townHead;
	while (townCurrent->next != NULL && townCurrent->next->population > newTown->population) {
		townCurrent = townCurrent->next;
	}

	insertTownAfter(townCurrent, newTown);

	return EXIT_SUCCESS;
}

int insertTownAfter(Town* townCurrent, Town* newTown) {
	newTown->next = townCurrent->next;
	townCurrent->next = newTown;

	return EXIT_SUCCESS;
}

Town* insertNewTownTree(Town* townTreeRoot, Town* newTownTree) {
	if (townTreeRoot == NULL)
		return newTownTree;

	if (newTownTree->population > townTreeRoot->population)
		townTreeRoot->left = insertNewTownTree(townTreeRoot->left, newTownTree);
	else if (newTownTree->population <= townTreeRoot->population)
		townTreeRoot->right = insertNewTownTree(townTreeRoot->right, newTownTree);

	return townTreeRoot;
}

int insertSortedNewCountryList(Country* townHead, Country* newCountry) {
	Country* countryCurrent = townHead;
	while (countryCurrent->next != NULL && strcmp(countryCurrent->next->name, newCountry->name) < 0) {
		countryCurrent = countryCurrent->next;
	}

	insertCountryAfter(countryCurrent, newCountry);

	return EXIT_SUCCESS;
}

int insertCountryAfter(Country* countryCurrent, Country* newCountry) {
	newCountry->next = countryCurrent->next;
	countryCurrent->next = newCountry;

	return EXIT_SUCCESS;
}

Country* insertNewCountryTree(Country* countryTreeRoot, Country* newCountry) {
	if (countryTreeRoot == NULL)
		return newCountry;

	if (strcmp(countryTreeRoot->name, newCountry->name) > 0)
		countryTreeRoot->left = insertNewCountryTree(countryTreeRoot->left, newCountry);
	else if (strcmp(countryTreeRoot->name, newCountry->name) <= 0)
		countryTreeRoot->right = insertNewCountryTree(countryTreeRoot->right, newCountry);

	return countryTreeRoot;
}

int printCountryList(Country* countryHeadList) {
	Country* currentCountry = countryHeadList->next;
	while (currentCountry != NULL) {
		printf("\n%s", currentCountry->name);
		printTownTree(currentCountry->townTreeRoot);
		currentCountry = currentCountry->next;
	}
}

int printCountryTree(Country* countryHeadTree) {
	if (countryHeadTree) {
		printCountryTree(countryHeadTree->left);
		printf("\n%s", countryHeadTree->name);
		printTownList(&countryHeadTree->townListHead);
		printCountryTree(countryHeadTree->right);
	}
	return EXIT_SUCCESS;
}

int printTownList(Town* townHeadList) {
	Town* currentTown = townHeadList->next;
	while (currentTown != NULL) {
		printf("\n\t%s %d", currentTown->name, currentTown->population);
		currentTown = currentTown->next;
	}
}

int printTownTree(Town* townHeadTree) {
	if (townHeadTree) {
		printTownTree(townHeadTree->left);
		printf("\n\t%s %d", townHeadTree->name, townHeadTree->population);
		printTownTree(townHeadTree->right);
	}
	return EXIT_SUCCESS;
}

