#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lineSize 300
#define wordSize 50
#define LOAD_FACTOR 0.7
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct listNode *list;
typedef list listPtr;
typedef struct hashTableItems *item;
typedef struct hashTable *table;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct listNode {
    char wordMeanings[wordSize];
    struct listNode *next;
};

struct hashTableItems {
    char word[wordSize];
    listPtr listPtr1;
};

struct hashTable {
    item item1;
    int emptyOrNot;
    int numOfItems;
};
//These global variables are used to calculate the number of collision that happens
int collision = 0;
int secondCollision = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions prototypes
int numberOfItemsInsideInputFile();

int isPrimeNumber(int number);

int hashTableSize(int numberOfItems);

void readingInputFileContents(table hashTable1, table hashTable2, int tableSize);

void insertNodeToLinkedList(listPtr tail, char wordMeaning[wordSize]);

list creatingLinkedList(char line[lineSize]);

void printItemInfo(item item1);

int checkLoadFactor(int counter, int size);

int hashFunction(char word[wordSize], int size);

int fillTable(table table, item item1, int size);

void initializeTable(table table1, int tableSize);

table rehashedTable(table hashTable1, int tableSize, int *new);

void printHashTable(table table1, int tableSize);

int calculateAsciiValue(char word[wordSize]);

int secondHashFunction(char word[wordSize], int tableSize);

table rehasedSecondTable(table table1, int tableSize, int *new);

int fillDoubleHashTable(table table, item item1, int tableSize);

int findPrimeNumber(int tableSize);

item findingWord(table t, char word[wordSize], int size, int number);

int deleteWord(table t, char word[wordSize], int size, int number);
void printHashTablesToFile(table t1, int size, FILE *output);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    printf("\t\t\t\t<< Welcome To Dictionary Management Program with Hashing >> \n");
    printf("\t\t\t--------------------------------------------------------------------------------\n\n");

    //calculating the number of words inside the file
    int numberOfItems = numberOfItemsInsideInputFile();
    //calculating the size of hash table
    int tableSize = hashTableSize(numberOfItems);

    table hashTable1 = (table) malloc(sizeof(struct hashTable) * tableSize);
    table hashTable2 = (table) malloc(sizeof(struct hashTable) * tableSize);

    //Make the hash tables empty
    initializeTable(hashTable1, tableSize);
    initializeTable(hashTable2, tableSize);

    //calling the readingInputFileContents function to read the file
    readingInputFileContents(hashTable1, hashTable2, tableSize);

    //output file pointer
    FILE *output;
    int choiceNum;
    while (1) {
        // menu choices
        printf("Please select one of the options below:\n");
        printf("1. Print hashed tables (including empty spots).\n");
        printf("2. Print out table size and the load factor. \n");
        printf("3. Print out the used hash functions\n");
        printf("4. Insert a new record to hash table (insertion will be done on both hash tables).\n");
        printf("5. Search for a specific word (specify which table to search in)\n");
        printf("6. Delete a specific record (from both tables).\n");
        printf("7. Compare between the two methods in terms of number of collisions occurred.\n");
        printf("8. Save hash table back to a file named saved_words.txt (of the double hashing)\n");
        printf("9.Exit\n\n");
        //printf("----------------------------------------------------------------------------------\n");
        printf("Choice number: ");
        //checking the validity of the choice
        if (scanf("%d", &choiceNum) == 1) {
            // Input is an integer
            if (choiceNum > 9 || choiceNum < 1) {
                printf("Invalid choice. Try again\n");
                printf("----------------------------------------------------------------------------------\n");
                continue;  // Skip the rest of the loop and go to the next iteration
            }
            //printf("You entered: %d\n", choiceNum);
        } else {
            // Input is not an integer
            printf("Invalid input. Please enter an integer.\n");
            // Clear the input buffer to prevent issues
            while (getchar() != '\n');
        }
        printf("-------------------------------------------------------------------------------------\n");
        // switch statement to execute user choice
        switch (choiceNum) {
            //Print hashed tables (including empty spots)
            case 1:
                printf("\t\t<< Hashed Table (Open Addressing - Quadratic Probing) >>\n");
                printf("----------------------------------------------------------------------------------\n");
                printf("Index\t  Statues\t\t Value\n\n");
                //calling the printHashTable function to print hash tables
                printHashTable(hashTable1, tableSize);
                printf("\nnumber of collision = %d\n",collision);
                printf("----------------------------------------------------------------------------------\n");
                printf("\t\t<< Hashed Table (Double Hashing) >>:\n");
                printf("----------------------------------------------------------------------------------\n");
                printf("Index\t  Statues\t\t Value\n\n");
                printHashTable(hashTable2, tableSize);
                printf("\nnumber of collision = %d\n1"
                       "",secondCollision);
                printf("----------------------------------------------------------------------------------\n");
                break;
            //Print out table size and the load factor.
            case 2:
                // print the table size and the load factor
                printf("Table size = %d\n", tableSize);
                //calculating the load factor value
                float loadFactor1 = (float) hashTable1->numOfItems / tableSize;
                printf("Load Factor for the first hashed table (Open Addressing - Quadratic Probing) = %0.2f\n",
                       loadFactor1);
                float loadFactor2 = (float) hashTable2->numOfItems / tableSize;
                printf("Load Factor for the second hashed table (Double hashing) = %0.2f\n", loadFactor2);
                printf("----------------------------------------------------------------------------------\n");
                break;

            //Print out the formula of hash functions which were used
            case 3:
                printf("1) The hash function for Quadratic Hashing is:\n");
                printf("h(key)=(StringHashing(key) + i * i) MOD tableSize\nWhere the tableSize = %d\n", tableSize);
                printf("\n");
                int n = findPrimeNumber(tableSize);
                printf("2) The hash Function for the Double Addressing is:\nh(key) = (StringHashing(key) + i* secondHashFunction(key)) MOD tableSize"
                       "\nWhere tableSize = %d\nsecondHashFunction(key) = (previousPrimeNum) - (StringHashing) MOD (previousPrimeNum),Where the previousPrimNum = %d"
                       "\n",
                       tableSize, n);
                printf("----------------------------------------------------------------------------------\n");
                break;

            //Insert a new word to hash table
            case 4:
                printf("Please enter a word to add to the hash table:\n");
                char word[wordSize];
                fflush(stdin);//to clear the buffer
                gets(word);
                printf("Please specify the number of meanings for the word:\n");
                int number;
                scanf("%d", &number);
                // creat list of meanings for the new word
                listPtr list = (listPtr) malloc(sizeof(struct listNode));
                listPtr pos = list;
                list->next = NULL;
                char meanings[lineSize];
                for (int i = 0; i < number; i++) {
                    printf("Enter meaning number [%d] for the word:\n", (i + 1));
                    fflush(stdin);
                    gets(meanings);//read meanings
                    insertNodeToLinkedList(list, meanings);//add the meanings to list
                    pos = pos->next;
                }
                // creat the node with entered word
                item item1 = (item) malloc(sizeof(struct hashTableItems));
                strcpy(item1->word, word);
                item1->listPtr1 = list;
                // insert the word to the table
                int res = fillTable(hashTable1, item1, tableSize);
                if (res == 0) { // Rehash needed (another table build)
                    int newSize;
                    table t = rehashedTable(hashTable1, tableSize, &newSize);
                    hashTable1->numOfItems = t->numOfItems;
                    // rebuild the table
                    hashTable1 = (table) realloc(t, sizeof(struct hashTable) * newSize);
                    tableSize = newSize;

                }
                // insert the node to the double table
                int res2 = fillDoubleHashTable(hashTable2, item1, tableSize);
                if (res2 == 0) {
                    // Rehash needed
                    int newSize2;
                    table t2 = rehasedSecondTable(hashTable2, tableSize, &newSize2);
                    hashTable2->numOfItems = t2->numOfItems;
                    hashTable2 = (table) realloc(t2, sizeof(struct hashTable) * newSize2);
                    tableSize = newSize2;
                }
                printf("The new word was inserted successfully!!!\n");
                printf("----------------------------------------------------------------------------------\n");
                break;

            case 5:
                //Search for a specific word (specify which table to search in).
                printf("Specify the table for your search:\n");
                printf("1)Open Addressing(Quadratic Probing)\n");
                printf("2)Double hashing table\n");
                int ch;
                scanf("%d", &ch);
                char foundWord[50];
                printf("Please enter the word you want to search for: ");
                fflush(stdin);
                gets(foundWord);
                if (ch == 1) {
                    findingWord(hashTable1, foundWord,tableSize,1);
                } else {
                   findingWord(hashTable2,foundWord, tableSize,2);
                }
                printf("----------------------------------------------------------------------------------\n");
                break;

                // Delete a specific word (from both tables)
            case 6:
                printf("Please enter the word you want to delete: ");
                fflush(stdin);
                char wordToDelete[50];
                gets(wordToDelete);//read the deleted word
                deleteWord(hashTable1, wordToDelete,tableSize,1);
                deleteWord(hashTable2,wordToDelete,tableSize,2);
                printf("The word was deleted successfully!!!\n");
                printf("----------------------------------------------------------------------------------\n");
                break;
            //Comparing
            case 7:
                printf("The number of collision that happened in open addressing were = %d\n", collision);
                printf("The number of collision that happened in Double addressing were = %d\n", secondCollision);
                //comparing
                if (collision > secondCollision) {
                    printf("Double hashing is the best\n");
                } else {
                    printf("Open addressing is the best\n");
                }
                printf("----------------------------------------------------------------------------------\n");
                break;

                //Save hash table back to a file named saved_words.txt of double hashing
            case 8:
                //open the output file (saved_words.txt)
                output = fopen("saved_words.txt", "w");
                if (output == NULL) {
                    printf("File Can not be opened!!\n");
                } else {
//                    fprintf(output, "\t\tData in Hash Table Using Quadratic Hashing\n");
//                    printHashTablesToFile(hashTable1, tableSize, output);
                    fprintf(output, "\t\t<<The contents of Hash Table Using Double Hashing>>\n");
                    printHashTablesToFile(hashTable2,tableSize, output);
                    fclose(output);
                    printf("The hashed function was saved to the file called [saved_words.txt] successfully!!!\n");
                }
                printf("----------------------------------------------------------------------------------\n");
                break;

                //exit from the program
            case 9:
                printf("<< THANKS FOR YOUR TIME!, see you again! >>\n");
                printf("----------------------------------------------------------------------------------\n");
                exit(0);
                break;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function to calculate the number of items that the hash function will contain
int numberOfItemsInsideInputFile() {
    int count = 0;
    FILE *inputFile;
    char line[lineSize];
    inputFile = fopen("words.txt", "r");//opening input file
    if (inputFile == NULL) {
        printf("File can not be open ! ");
        exit(0);
    }
    while (fgets(line, sizeof(line), inputFile))//reading each line inside the file
        count++;
    return count;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function to determine the hash table size
int hashTableSize(int numberOfItems) {
    numberOfItems *= 2;
    while (!isPrimeNumber(numberOfItems)) { //This while loop is used to find a prime number
        numberOfItems++;
    }
    return numberOfItems;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function that checking if the number is prime number or not
int isPrimeNumber(int number) {
    int primeNum = 1;
    if (number == 2) {
        return 0;
    }
    for (int i = 2; i <= number / 2; i++) {
        if (number % i == 0) {
            primeNum = 0; //the number is not prime
            break;
        }
    }
    return primeNum;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void readingInputFileContents(table hashTable1, table hashTable2, int tableSize) {
    FILE *inputFile;
    listPtr list;
    char *token;
    char wordMeanings[lineSize];
    char line[lineSize];
    char word[wordSize];
    int newSize1;
    int newSize2;
    // open the file
    inputFile = fopen("words.txt", "r");
    if (inputFile == NULL) {
        printf("File can not be open!");
        exit(0);
    }
    //read line by line
    while (fgets(line, sizeof(line), inputFile)) {
        token = strtok(line, ":");
        strcpy(word, token);
        token = strtok(NULL, ",");
        strcpy(wordMeanings, token);
        // pass the line of meanings to the method to split then store them
        list = creatingLinkedList(wordMeanings);

        item items = (item) malloc(sizeof(struct hashTableItems));
        strcpy(items->word, word);
        items->listPtr1 = list;

        int rehashNeeded1 = fillTable(hashTable1, items, tableSize);
        if (rehashNeeded1 == 0) {
            // rehash is needed
            table t = rehashedTable(hashTable1, tableSize, &newSize1);
            // rebuild the table
            hashTable1 = (table) realloc(t, sizeof(struct hashTable) * newSize1);

        }
        int rehashNeeded2 = fillDoubleHashTable(hashTable2, items, tableSize);
        if (rehashNeeded2 == 0) {
            // rebuild the table
            table t2 = rehasedSecondTable(hashTable2, tableSize, &newSize2);
            hashTable2 = (table) realloc(t2, sizeof(struct hashTable) * newSize2);
        }
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function which creates a list of word meanings
list creatingLinkedList(char line[lineSize]) {
    char *token;
    listPtr head;

    head = (listPtr) malloc(sizeof(struct listNode));
    if (head == NULL) {
        printf("Out Of Memory Space!!");
        exit(0);
    }

    listPtr tail = head;
    head->next = NULL;
    token = strtok(line, "#");//cutting the line depending on char #
    insertNodeToLinkedList(tail, token);//insert the word meaning in the list
    while (token != NULL) {
        tail = tail->next;
        token = strtok(NULL, "#");
        if (token == NULL) {
            break;
        }
        //insert the word meaning in the list
        insertNodeToLinkedList(tail, token);
    }
    return head;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function that creating new node which contains the meaning of word
void insertNodeToLinkedList(listPtr tail, char wordMeaning[wordSize]) {
    listPtr ptr = (listPtr) malloc(sizeof(struct listNode));
    if (ptr == NULL) {
        printf("Out Of Memory Space!!");
    }
    strcpy(ptr->wordMeanings, wordMeaning);
    ptr->next = tail->next;
    tail->next = ptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function that prints the linked list
void printItemInfo(item item1) {
    printf(" ");
    printf("%s: ", item1->word);
    //printf("The meanings of this word : \n");
    //printList(item1->listPtr1);
    listPtr p = item1->listPtr1->next;
    if (p == NULL) {
        printf("Empty list!!!\n");
    }
    while (p != NULL) {
        printf("%s ", p->wordMeanings);
        p = p->next;
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A Hash function to decide which index will be used
int hashFunction(char word[wordSize], int size) {
    int hashValue = 0;
    int i = 0;
    while (word[i] != '\0') {
        hashValue += word[i];
        i++;
    }
    return (hashValue % size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function to decide if we will need the rehash or not depending on load factor
int checkLoadFactor(int counter, int size) {
    float loadFactor = (float) counter / size;
    if (loadFactor > LOAD_FACTOR) {
        return 0;
    } else {
        return 1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function to fill the hash table
int fillTable(table table, item item1, int size) {// Open addressing
    //Quadratic Hashing
    // int flag = 0;
    int index = hashFunction(item1->word, size); // index in which could be stored
    int i = 1;
    // if the number not part full insert
    int loadFactorValue = checkLoadFactor(table->numOfItems, size);
    if (loadFactorValue) {
        // flag = 1;
        if (table[index].emptyOrNot == 0 || table[index].emptyOrNot == -1) { // Empty
            table[index].item1 = item1;
            table[index].emptyOrNot = 1; //occupied
            table->numOfItems++;
        } else {
            // collision
            while (table[index].emptyOrNot == 1) {//full
                index = (index + i * i) % size;
                collision++;
                if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
                    table[index].item1 = item1;
                    table[index].emptyOrNot = 1;
                    table->numOfItems++;
                    break;
                }
                i++;
            }
        }
    } else { // the last value in which load factor fail
        if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
            table[index].item1 = item1;
            table[index].emptyOrNot = 1;
            table->numOfItems++;
        } else {
            while (table[index].emptyOrNot == 1) {//full collision
                index = (index + i * i) % size;
                collision++;
                if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
                    table[index].item1 = item1;
                    table[index].emptyOrNot = 1;
                    table->numOfItems++;
                    break;
                }
                i++;
            }
        }
        return 0;//  Rehash needed
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
table rehashedTable(table hashTable1, int tableSize, int *new) {
    int newTableSize = hashTableSize(tableSize * 2); //new table size
    *new = newTableSize;
    int j = 1;
    table newTable = (table) malloc(sizeof(struct hashTable) * newTableSize);
    initializeTable(newTable, newTableSize);
    for (int i = 0; i < tableSize; i++) {
        if (hashTable1[i].emptyOrNot == 1) {
            // move all data to the new table
            int index = hashFunction(hashTable1[i].item1->word, newTableSize);
            if (newTable[index].emptyOrNot == -1 || newTable[index].emptyOrNot == 0) {
                newTable->numOfItems++;
                newTable[index].item1 = hashTable1[i].item1;
                newTable[index].emptyOrNot = 1;
            } else {
                while (newTable[index].emptyOrNot == 1) {//full
                    index = (index + j * j) % tableSize;
                    if (newTable[index].emptyOrNot == -1 || newTable[index].emptyOrNot == 0) {
                        newTable[index].item1 = hashTable1[i].item1;
                        newTable[index].emptyOrNot = 1;
                        newTable->numOfItems++;
                        break;
                    }
                    j++;
                }
            }
        }
    }
    return newTable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initializeTable(table table1, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        table1[i].emptyOrNot = 0; // initialize the table
    }
    table1->numOfItems = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printHashTable(table table1, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        if (table1[i].emptyOrNot == 1) {
            printf(" [%d]\t", i);
            printf("Full spot\t ", table1[i].emptyOrNot);
            printItemInfo(table1[i].item1);
            // printf("\n");
            //printf("---------------------------------------------------\n");
        } else if (table1[i].emptyOrNot == 0) {
            printf(" [%d]\t", i);
            printf("Empty spot\t", table1[i].emptyOrNot);
            printf("\t-------\n");
            //printf("\n");
        } else {
            printf(" [%d]\t", i);
            printf("Deleted word\n");
            //printf("---------------------------------------------------\n");
            //printf("\n");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
table rehasedSecondTable(table table1, int tableSize, int *new) {
    int newSize = hashTableSize(2 * tableSize); // First prime greater then the double of size
    *new = newSize;
    int j = 1;
    table newTable = (table) malloc(sizeof(struct hashTable) * newSize);//allocation
    initializeTable(newTable, newSize);//make it empty
    for (int i = 0; i < tableSize; i++) {
        if (table1[i].emptyOrNot == 1) {
            int index = hashFunction(table1[i].item1->word, newSize);
            if (newTable[index].emptyOrNot == -1 || newTable[index].emptyOrNot == 0) {
                newTable->numOfItems++;
                newTable[index].item1 = table1[i].item1;
                newTable[index].emptyOrNot = 1;
            } else {
                while (newTable[index].emptyOrNot == 1) {//full
                    index = (index + (j) * secondHashFunction(table1[i].item1->word, tableSize)) % tableSize;
                    // new index
                    if (newTable[index].emptyOrNot == -1 || newTable[index].emptyOrNot == 0) {
                        newTable[index].item1 = table1[i].item1;
                        newTable[index].emptyOrNot = 1;
                        newTable->numOfItems++;
                        break;
                    }
                    j++;
                }
            }
        }
    }
    return newTable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int secondHashFunction(char word[wordSize], int tableSize) {
    tableSize--;
    while (!isPrimeNumber(tableSize)) {
        tableSize--;
    }
    return (tableSize - (calculateAsciiValue(word) % tableSize));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int calculateAsciiValue(char word[wordSize]) {
    int asciiValue = 0;
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == '\0') {
            break;
        }
        asciiValue += word[i];
    }
    return asciiValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int fillDoubleHashTable(table table, item item1, int tableSize) {
    fflush(stdin);
    int flag = 0;
    int index = hashFunction(item1->word, tableSize);
    int i = 1;
    if (checkLoadFactor(table->numOfItems, tableSize)) {
        flag = 1;
        if (table[index].emptyOrNot == 0 || table[index].emptyOrNot == -1) { // Empty
            table[index].item1 = item1;
            table[index].emptyOrNot = 1; //occupied
            table->numOfItems++;
        } else {
            // collision
            while (table[index].emptyOrNot == 1) {//full
                index = (index + i * secondHashFunction(item1->word, tableSize)) % tableSize;
                secondCollision++;
                if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
                    table[index].item1 = item1;
                    table[index].emptyOrNot = 1;
                    table->numOfItems++;
                    break;
                }
                i++;
            }
        }
    } else {
        // the last value in which load factor fail
        if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
            table[index].item1 = item1;
            table[index].emptyOrNot = 1;
            table->numOfItems++;
        } else {
            while (table[index].emptyOrNot == 1) {//full
                index = (index + i * secondHashFunction(item1->word, tableSize)) % tableSize;
                secondCollision++;
                if (table[index].emptyOrNot == -1 || table[index].emptyOrNot == 0) {
                    table[index].item1 = item1;
                    table[index].emptyOrNot = 1;
                    table->numOfItems++;
                    break;
                }
                i++;
            }
        }
        return 0;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int findPrimeNumber(int tableSize) {
    tableSize--;
    while (!isPrimeNumber(tableSize)) {
        tableSize--;
    }
    return tableSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
item findingWord(table t, char word[wordSize], int size, int number) {
    if (number == 1) {
        int index = hashFunction(word, size);
        int j = 0;
        while (t[index].emptyOrNot == 1) {//full
            index = (index + j * j) % size;
            if (strcmp(t[index].item1->word, word) == 0) {
                printItemInfo(t[index].item1);
                return t[index].item1;
            }
            j++;
        }
        printf("There is no word like this in the table .\n");
    }
    //looking for word in double hash table
    if (number == 2) {
        int index = hashFunction(word, size);
        int j = 0;
        while (t[index].emptyOrNot == 1) {//full
            index = (index + (j) * secondHashFunction(word, size)) % size;
            if (strcmp(t[index].item1->word, word) == 0) {
                printItemInfo(t[index].item1);
                return t[index].item1;
            }
            j++;
        }
        printf("There is no word like this in the table .\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int deleteWord(table t, char word[wordSize], int size, int number) {
    if (number == 1) {
        int index = hashFunction(word, size);
        int j = 0;
        while (t[index].emptyOrNot == 1) {//full
            index = (index + j * j) % size;
            if (strcmp(t[index].item1->word, word) == 0) {
                t[index].item1 = NULL;
                t[index].emptyOrNot = -1;
                return 1;
            }
            j++;
        }
    }
    if (number == 2) {
        int index = hashFunction(word, size);
        if (strcmp(t[index].item1->word, word) == 0) {
            t[index].item1 = NULL;
            t[index].emptyOrNot = -1;
            return 1;
        }
        int j = 1;
        while (t[index].emptyOrNot == 1) {//full
            index = (index + (j) * secondHashFunction(word, size)) % size;
            if (strcmp(t[index].item1->word, word) == 0) {
                t[index].item1 = NULL;
                t[index].emptyOrNot = -1;
                return 1;
            }

            j++;
        }
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printHashTablesToFile(table t1, int size, FILE *output) {

    for (int i = 0; i < size; i++) {
        if (t1[i].emptyOrNot == 1) {
            fprintf(output, "For index : %d\t", i);
            fprintf(output, "Status is : Full \t", t1[i].emptyOrNot);
            fprintf(output, "%-35s \n", t1[i].item1->word);
            fprintf(output, "---------------------------------------------------\n");
        } else if (t1[i].emptyOrNot == 0) {
            fprintf(output, "For index : %d\t", i);
            fprintf(output, "Status is : Empty spot \t", t1[i].emptyOrNot);
            fprintf(output , "---------------------------------------------------\n");
        } else {
            fprintf(output, "For index : %d\t", i);
            fprintf(output, "Status  is :Deleted \n");
            fprintf(output, "---------------------------------------------------\n");
        }
    }

}
