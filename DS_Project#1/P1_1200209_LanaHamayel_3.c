//Lana Amjad Hamayel
//1200209
//section:3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node
{
    char data[31];
    struct node *prev;
    struct node *next;
};
///////////////////////////////////
struct letterNode
{
    char letter;
    struct node *next;
};
/////////////////////////////////
struct node *head = NULL;
struct node *tail = NULL;
struct node *head2 = NULL;
struct node *tail2 = NULL;
struct node* ptr2 = NULL;


struct letterNode *myArray[90] = {NULL};
/////////////////////////////////////////
//function prototypes
void insert(char word[]);
void printList();
void createHeader ();
void fillArray();
void radixSort (int max,int count);
int getLongestWord( char fileName [40]);
void createLinkedList2();
void printList2() ;
void deleteNode (char word [], int max,int count);
void printToFile() ;
///////////////////////////////////////////
int main()
{
    printf("\t\t\t\t<< Welcome to Radix Sort Program >> \n");
    printf("\t\t\t--------------------------------------------------\n\n");
    createHeader();
    fillArray();
    char word[31];
    char newWord [31];
    char deletedWord [31];
    char addWord [31];
    char fileName [40];
    int length=0,count=0;

    //Reading strings from file
    FILE *input;
    printf("Please enter the input file name: ");
    scanf("%s",fileName);
    int max=getLongestWord(fileName);
    input= fopen(fileName,"r");
    while(input==NULL)
    {
        printf("Try Again! The file does not exist.\n");
        printf("\n");
        printf("please enter the input file name: ");
        scanf("%s",fileName);
        input = fopen(fileName,"r");
    }

    while (fgets(word, sizeof(word), input) != NULL)
    {
        // Remove the newline character from the end of the word
        word[strcspn(word, "\n")] = '\0';
        length = strlen(word);
        //adding spaces to each words in the string
        int numOfSpaces = max-length;
        for (int i=length; i<=numOfSpaces+(length-1); i++)
        {
            word[i] = ' ';
        }
        strlwr(word);
        insert(word);
        count++;
    }
    printf("----------------------------------------------------------------------------------\n");
    int choiceNum;
    while (1)
    {
        printf("Please select one of the options below:\n");
        printf("1. Load strings\n");
        printf("2. Print the strings before sorting\n");
        printf("3. Sort the strings\n");
        printf("4. Print the sorted strings\n");
        printf("5. Add a new word to the list of sorted	strings(and sort it)\n");
        printf("6. Delete a word from the sorted strings\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n");
        printf("----------------------------------------------------------------------------------\n");
        printf("Choice number = ");
        //checking the validity of the choice
        scanf("%d",&choiceNum);
        if (choiceNum > 8 || choiceNum < 1)
        {
            printf("Invalid choice. Try again\n");
            printf("----------------------------------------------------------------------------------\n");
            continue;  // Skip the rest of the loop and go to the next iteration
        }
        printf("----------------------------------------------------------------------------------\n");
        switch(choiceNum)
        {
        case 1:
            input= fopen(fileName,"r");
            printf("<< Strings inside the file are: >>\n");
            while (fgets(word, sizeof(word), input) != NULL)
            {
                // Remove the newline character from the end of the word
                word[strcspn(word, "\n")] = '\0';
                printf("%s \n",word);
            }
            printf("----------------------------------------------------------------------------------\n");
            break;

        case 2:
            printf("<< Strings before Sorting are: >>\n");
            printList();
            printf("----------------------------------------------------------------------------------\n");
            break;


        case 3:
            radixSort(max,count);
            printf("Radix Sort, Done! Strings were sorted successfully!\n");
            printf("----------------------------------------------------------------------------------\n");
            break;

        case 4:
            printf("<< Strings after Sorting are: >>\n");
            //   radixSort(max,count);
            printList2();
            printf("----------------------------------------------------------------------------------\n");
            break;


        case 5:
            printf("Please enter the string that you want to be sorted: ");
            scanf("%s",newWord);
            addWord[strcspn(newWord,"\n")] = '\0';
            strlwr(addWord);
            int newWordLength = strlen(newWord);
            count++;
            //checking if the new word it has a max size or not
            if (newWordLength >= max)
            {
                max = newWordLength;
                struct node *newPtr = head->next;
                while (newPtr != NULL)
                {
                    strncpy(addWord,newPtr->data,strlen(newPtr->data)+1);
                    int length = strlen(newPtr->data);
                    int numOfSpaces = max - length;
                    for (int i=length; i<=numOfSpaces+(length-1); i++)
                    {
                        addWord[i] = ' ';
                    }
                    strncpy(newPtr->data,addWord,strlen(addWord) +1 );
                    newPtr=newPtr->next;
                }
            }

            else
            {
                int length2 = strlen(newWord);
                int numOfSpaces = max - length2;
                for (int i=length2; i<=numOfSpaces+(length2-1); i++)
                {
                    newWord[i] = ' ';
                }
            }
            insert(newWord);
            printf("<< Strings before sorting after adding a new String are: >>\n");
            printList();
            radixSort(max,count);
            printf("<< Strings after sorting after adding a new String are: >>\n");
            printList2();
            printf("----------------------------------------------------------------------------------\n");
            break;


        case 6:
            printf("Enter the string that you want to be deleted: \n");
            printList2();
            printf("word = ");
            scanf("%s",&deletedWord);
            // printf("%s\n",deletedWord);
            printf("<< Strings after deletion are: >>\n");
            deleteNode(deletedWord, max,count);
            printf("----------------------------------------------------------------------------------\n");
            break;


        case 7:
            printToFile();
            printf("----------------------------------------------------------------------------------\n");

            break;

        case 8:
            printf("<< Bye, see you again! >>\n");
            exit(0);
        }
    }
    fclose(input);
    free(ptr2);
    free(head2);
    free(head);
    return 0;
}

///////////////////////////////////////////////////////
//function to create a header
void createHeader ()
{
    head = (struct node *)malloc(sizeof(struct node));
    head->next=NULL;
    head->prev=NULL;
    tail=head;
}

///////////////////////////////////////////////////////////
//function to insert nodes
void insert(char word [])
{
    struct node* ptr1 = (struct node*)malloc(sizeof(struct node));
    if (ptr1 == NULL)
    {
        printf("Failed to allocate memory. No memory available.\n");
        exit(0);
    }
    int length = strlen(word);
    strncpy(ptr1->data, word, length+1);
    if(tail != NULL)
    {
        tail -> next = ptr1;
        ptr1-> next = NULL;
        ptr1-> prev = tail;
        tail=tail->next;
    }
}
//////////////////////////////////////////////////////////////
//function to print list before sort
void printList()
{
    struct node *ptr =head->next;
    while (ptr != NULL)
    {
        printf("%s\n",ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}
////////////////////////////////////////////////////////////
//function to fill the array of linked list
void fillArray()
{
    myArray[0]= (struct letterNode*)malloc(sizeof(struct letterNode));
    myArray[0]->next=NULL;
    myArray[0]->letter =' ';
    for (int k=0; k<=90; k++)
    {
        myArray[k+1]= (struct letterNode*)malloc(sizeof(struct letterNode));
        myArray[k+1]->next=NULL;
        myArray[k+1]->letter='!'+k;
    }
}
/////////////////////////////////////////////////////////
void radixSort (int max,int count)
{
    int wordLength = max;
    int resetCount = count;
    char word [wordLength];
    struct node *ptr = head->next;
    //this loop to add spaces to words depending on the maximum word length
    while (max > 0)
    {
        //this loop to add spaces to all words
        // count = number of words
        while (count > 0)
        {
            struct node* ptr1 = (struct node*)malloc(sizeof(struct node));
            strncpy(ptr1->data, ptr->data,wordLength+1);
            ptr1->next = NULL;
            ptr1->prev = NULL;

            strncpy(word, ptr->data,wordLength+1);
            char letter = word[max-1];
            int index =0;
            if (letter != ' ')
            {
                index = (letter-'!')+1;
            }
            //if the pointer[index] points to one word
            if(myArray[index]->next == NULL)
            {
                myArray[index]->next = ptr1;

            }
            //if the pointer[index] points to many words
            else
            {
                struct node *myPtr = myArray[index]->next ;
                while (myPtr->next != NULL)
                {
                    myPtr = myPtr->next;
                }
                myPtr->next =ptr1;
                ptr1->prev=myPtr;
                ptr1->next=NULL;
            }

            if(ptr->next != NULL)
            {
                ptr=ptr->next;
            }
            count--;

        }
        createLinkedList2();
        ptr= head2->next;
        count = resetCount;
        max=max-1;
    }
}
//////////////////////////////////////////////////////////
void createLinkedList2()
{
    head2 = (struct node *)malloc(sizeof(struct node));
    head2->next=NULL;
    head2->prev=NULL;
    tail2=head2;
    ptr2=head2;

    for (int i=0 ; i<=90; i++)
    {
        if(myArray[i] != NULL && myArray[i]->next != NULL)
        {
            tail2 = myArray[i]->next;
            ptr2->next = tail2;
            tail2->prev = ptr2;
            myArray[i]->next = NULL;
            while(tail2->next != NULL)
            {
                tail2 = tail2->next;
            }
            ptr2 = tail2;
        }
    }

}
/////////////////////////////////////////////////////////
//function to print linked list after sorting
void printList2()
{
    struct node *ptr =head2->next;
    while (ptr != NULL)
    {
        printf("%s\n",ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}
////////////////////////////////////////////////////////
int getLongestWord( char fileName [40])
{
    FILE *input;
    char word [31];
    int max=0;

    input= fopen(fileName,"r");
    while(input==NULL)
    {
        printf("Try Again! The file does not exist.\n");
        printf("\n");
        printf("please enter the input file name: ");
        scanf("%s",fileName);
        input = fopen(fileName,"r");
    }

    while (fgets(word, sizeof(word), input) != NULL)
    {
        // Remove the newline character from the end of the word
        word[strcspn(word, "\n")] = '\0';
        int length = strlen(word);
        //printf("length = %d",length);
        if (length>max)
        {
            max=length;
        }

    }
    return max;
}
//////////////////////////////////////////////////////////////////////
//function to delete nodes from linked list
void deleteNode (char word [], int max,int count)
{
    struct node *pointer = head2->next;
    word[strcspn(word, "\n")] = '\0';
    int length = strlen(word);
    int numOfSpaces = max-length;
    for (int i=length; i<=numOfSpaces+(length-1); i++)
    {
        word[i] = ' ';
    }
    while (pointer->next != NULL)
    {
        if(strcmp(pointer->data,word) != 0)
        {
            pointer=pointer->next;
        }
        else
        {
            struct node *tempPrev  = pointer->prev;
            struct node *tempNext  = pointer->next;

            tempPrev ->next = tempNext;
            tempNext->prev = tempPrev;
            free(pointer);
            printList2();

            return;
        }
    }

    if(strcmp(pointer->data,word)== 0)
    {
        //printf("\n pointer data = %s\n",pointer->data);

        struct node *lastNode = pointer->prev;
        lastNode ->next = NULL;
    }
    else
    {
        printf("This word does not exist!\n");
        return;

    }

    //printf("ptr dtaaaaa: %s",pointer->data);
    printf("----------------------------------------------------------------------------------\n");
    printList2();
    count--;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//function to print the output to a file
void printToFile()
{
    FILE *output;
    struct node *p=head2->next;
    printf("Please enter the file name with its path: ");

    char outputFile[30];
    scanf("%s", outputFile);  // Limit the input to 29 characters to prevent buffer overflow
    output = fopen(outputFile, "w");
    if (output == NULL)
    {
        printf("ERROR! Failed to open the output file.\n");
        return;
    }
    while (p != NULL)
    {
        fprintf(output, "%s\n", p->data);
        p = p->next;
    }
    printf("<< Strings were saved on %s successfully! Check out!\n",outputFile);

    fclose(output);

}
//////////////////////////////////////////////////////////////////////////////////////////////////
