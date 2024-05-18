#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node {
    char data[31];
    struct node *prev;
    struct node *next;
};

struct letterNode {
    char letter;
    struct node *next;
};

struct node* head = NULL;
struct node* tail = NULL ;


void insert( char word[]);
void printList(struct node* head);



int main() {

    char word[31];
    char fileName [40];
    FILE *input;
    printf("Please enter the input file name:\n");
    scanf("%s",fileName);
    input= fopen(fileName,"r");
    while(input==NULL){
        printf("Sorry! The file does not exist.\n");
        printf("Try Again!\nplease enter the input file name:\n");
        scanf("%s",fileName);
        input = fopen(fileName,"r");
    }
//int i=0;
 while (fgets(word, sizeof(word), input) != NULL) {
        // Remove the newline character from the end of the word
        word[strcspn(word, "\n")] = '\0';
        strlwr(word);
        insert(word);
       // i++;
    }
    printList(head);
//    printf("i= %d",i);

return 0;
}

void insert(char word []) {
    struct node* ptr1 = (struct node*)malloc(sizeof(struct node));
    if (ptr1 == NULL){
        printf("Failed to allocate memory. No memory available.\n");
        exit(0);
    }
    int length = strlen(word);
    strncpy(ptr1->data, word, length+1);
    ptr1->next = NULL;
    ptr1->prev = NULL;

    if (head == NULL) {
            head = ptr1;
            tail = ptr1;
    }
    else
    {
        tail -> next = ptr1;
        ptr1-> next = NULL;
        ptr1-> prev = tail;
        tail = tail -> next;
    }
}

void printList(struct node* ptr) {
    while (ptr != NULL) {
        printf("%s\n",ptr->data);
        ptr = ptr->next;
    }
    printf("\n");

}




