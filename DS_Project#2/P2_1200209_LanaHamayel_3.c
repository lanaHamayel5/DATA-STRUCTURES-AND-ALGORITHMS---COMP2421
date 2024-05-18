//Lana Amjad Hamayel
//1200209
//section 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 200
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define a struct node to read equations from file and store them on a linked list
typedef struct pStackNode pStackNode;
struct node
{
    char equation[SIZE];
    struct node *next;
};
// define a head pointer
struct node *head = NULL;
struct node *tail = NULL;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define an array of stack with size 200
int top = -1;
char arrOfStack[SIZE];
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define a struct of expTreeNode to create the tree and evaluate it after
struct expTreeNode
{
    char opretor[10];
    struct expTreeNode *left;
    struct expTreeNode *right;
};
//define a root pointer
struct expTreeNode *root = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define a stack node which used to build exp tree
int top2 = -1;
struct stackNode
{
    struct expTreeNode *tree;
};
struct stackNode *stackToTree[SIZE];
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//functions prototypes definitions
void createHeader();
void insert(char eq[]);
void push(char opreation);
char pop();
int isOpretor(char index);
int priorityOfOperators(char index);
int openingBracket(char index);
int closingBracket(char index);
int matchingBrackets(char index1, char index2);
void convertInfixToPostfix(int , FILE *);
int checkIsVaild(char expression[]);
void removeSpacesAndSpecialChars(char equation[]);
struct expTreeNode *createTree(char operator[10], struct stackNode *left, struct stackNode *right);
void fillStackToTree(char postfix[]);
void printPostfix(char postfix[]);
void printTree();
void pushToStructStack(struct expTreeNode *expTree);
struct expTreeNode *newNode(char data[10]);
int evaluateTree(struct expTreeNode *root);
void printList();
void printToFile();
void printPostfixToFile(char postfix[], FILE * fileName);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    printf("\t\t\t\t<< Welcome To Binary Arithmetic Expression Evaluator Program >> \n");
    printf("\t\t\t--------------------------------------------------------------------------------\n\n");
    createHeader();//create a head node for linked list
    char fileName[40];
    char equation[SIZE];
    FILE *input;
    int choiceNum;
    while (1)
    {
        // menu choices
        printf("Please select one of the options below:\n");
        printf("1. Read equations\n");
        printf("2. Print equations\n");
        printf("3. Print Expression tree\n");
        printf("4. Print Postfix expressions\n");
        printf("5. Save to output file(Postfix and results)\n");
        printf("6. Exit\n");
        printf("----------------------------------------------------------------------------------\n");
        printf("Choice number =  ");

        //scanf("%d",&choiceNum);
        //checking the validity of the choice
        if (scanf("%d", &choiceNum) == 1) {
            // Input is an integer
            if (choiceNum > 6 || choiceNum < 1)
        {
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
        printf("----------------------------------------------------------------------------------\n");
        // switch statement to execute user choice
        switch(choiceNum)
        {
            // reading equations from file
            case 1:
                printf("Please enter the input file name: ");
                scanf("%s", fileName);
                //opening file
                input = fopen(fileName, "r");
               //this loop is keeping rotate until the user enter the file name correctly
                while (input == NULL)
                {
                    printf("Try Again! The file does not exist.\n");
                    printf("\n");
                    printf("please enter the input file name: ");
                    scanf("%s", fileName);
                    input = fopen(fileName, "r");
                }
                //To move on all the equations that are inside the file
                while (fgets(equation, sizeof(equation), input) != NULL)
                {
                    // Remove the newline character from the end of the word
                    equation[strcspn(equation, "\n")] = '\0';
                    //to remove any space of special char from the equations
                    removeSpacesAndSpecialChars(equation);
                    //create a node to store each equation on it at linked list
                    insert(equation);
                }
                printf("<< Equations were read successfully! >>\n");
                printf("----------------------------------------------------------------------------------\n");
                break;
                //print equation from a linked list
            case 2:
                printf("<< Equations are: >>\n");
                printList();
                printf("----------------------------------------------------------------------------------\n");
                break;
                //print the evaluation of the exp tree
            case 3:
                printf("<< Expression tree result for equations are: >>\n");
                convertInfixToPostfix(2, NULL);//calling convertInfixToPostfix(2) function
                printf("----------------------------------------------------------------------------------\n");
                break;
            //print the postfix expressions
            case 4:
                printf("<< Postfix expression for equations are: >>\n");
                convertInfixToPostfix(1, NULL);//calling convertInfixToPostfix(1) function
                printf("----------------------------------------------------------------------------------\n");
                break;
             //print exp tree evaluation and postfix expression to output file
            case 5:
                printf("<< Save to File >>\n");
                FILE *output;
                printf("Please enter the file name with its extinsion: ");
                char outputFile[30];
                scanf("%s", outputFile);  // Limit the input to 29 characters to prevent buffer overflow
                output = fopen(outputFile, "w");//open the output file
                if (output == NULL)
                {
                    printf("ERROR! Failed to open the output file.\n");
                    return 0;
                }
                convertInfixToPostfix(3, output);//calling  convertInfixToPostfix(3, output)
                printf("<< Postfix expressions and expression tree evaluations were saved on %s successfully! Check out!\n",outputFile);
                fclose(output);
                printf("----------------------------------------------------------------------------------\n");
                break;
                //exit from the program
            case 6:
                printf("<< THANKS FOR YOUR TIME!, see you again! >>\n");
                printf("----------------------------------------------------------------------------------\n");
                exit(0);
                break;
        }
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//this function is used to create the header node for the linked list
void createHeader()
{
    head = (struct node *) malloc(sizeof(struct node));
    head->next = NULL;
    tail = head;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// this function is to insert a new node for the linked list
void insert(char eq[])
{
    struct node *ptr1 = (struct node *) malloc(sizeof(struct node));
    if (ptr1 == NULL)
    {
        printf("Failed to allocate memory. No memory available.\n");
        exit(0);
    }
    int length = strlen(eq);
    //cope the equation to node which was created
    strncpy(ptr1->equation, eq, length + 1);
    if (tail != NULL)
    {
        //connecting nodes with each other
        tail->next = ptr1;
        ptr1->next = NULL;
        tail = tail->next;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// this function is used to print the linked list by moving into nodes(node by node)
void printList (){
    struct node *ptr = head->next;
    int i=1;
    while (ptr != NULL)
    {
        //moving from node to other and print its data
        printf("Equation [%d] = %s\n",i, ptr->equation);
        printf("\n");
        ptr = ptr->next;
        i++;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//this function is used to push data to the stack,it was using while we convert the equation from the infix to postfix expression
void push(char opreator)
{
    if (top == SIZE)
    {
        printf("Overflow! STACK is full.\n");
    }
//push the operator and increment the top;
    arrOfStack[++top] = opreator;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//this function is used to pop data from the stack, it was using while we convert the equation from the infix to postfix expression
char pop()
{
    if (top == -1)
    {
        return -1;
    }
    else
    {
        //pop the operator and decrement the top;
        return arrOfStack[top--];
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to push data from type tree to stackToTree
void pushToStructStack(struct expTreeNode *expTree)
{
    //allocate node in mem
    struct stackNode *node = (struct stackNode *) malloc(sizeof(struct stackNode));
    //enter the data from type tree into
    node->tree = expTree;
    if (top2 == SIZE)
    {
        printf("Overflow! STACK is full.\n");
    }
    //push the operator and decrement the top;
    stackToTree[++top2] = node;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to pop data from type tree to stackToTree
struct stackNode *pop2()
{
    return stackToTree[top2--];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//this function is used to check if the array char which was passed it is an opretor or not
int isOpretor(char index)
{
    //it is opretor return 1 else return 0
    if (index == '+' || index == '-' || index == '*' || index == '/' || index == '%')
        return 1;
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to check the priority of the array char which was passed
int priorityOfOperators(char index)
{
    //check higher operator
    if (index == '*' || index == '/' || index == '%')
        return 2;
    //check middle operator
    if (index == '+' || index == '-')
        return 1;
    //check lowest operator
    if (index == '(')
        return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////this function check if the array char which was passed is opening Bracket or not
//int openingBracket(char index) //boolean unction to indicate whether the character is an opening bracket ( or [.
//{
//    if (index == '(')
//        return 1;
//    return 0;
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////this function check if the array char which was passed is closing Bracket or not
//int closingBracket(char index) //boolean unction to indicate whether the character is an closing bracket ) or ].
//{
//    if (index == ')')
//        return 1;
//    return 0;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function check if the array char which was passed is matching Brackets or not
//int matchingBrackets(char index1, char index2)
//{
//    if (index1 == '(' && index2 == ')')
//        return 1;
//    return 0;
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void printStack()
//{
//    printf("Stack contents: ");
//    for (int i = 0; i <= top; i++)
//    {
//        printf("%c ", arrOfStack[i]);
//    }
//    printf("\n");
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//char peek()
//{
//    if (top < 0)
//    {
//        printf("Stack Empty\n");
//        exit(EXIT_FAILURE);
//    }
//    return arrOfStack[top]; // Here to retrieves and returns the element at the top of the stack without modifying the stack itself.
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to convert the infix to postfix expression
void convertInfixToPostfix(int n, FILE * outFileName)
{
    char c;
    //this pointer move on the linked list node by node starting from the head node
    struct node *listPtr = head->next;
    //define 2D array the row to store the number of each equation and the column to store the equation itself
    char copiedEquation[SIZE][SIZE];
    int i = 0;
    //while do not reach the last node on linked list then move.
    while (listPtr != NULL)
    {
        //checking the validly of equations using checkIsVaild() function
        if (checkIsVaild(listPtr->equation))
        {
            //if it is valid then copy what this node has to the 2D array
            strncpy(copiedEquation[i], listPtr->equation, strlen(listPtr->equation) + 1);
            i++;
        }
        else
        {
            //if equation not valid the print a non valid equation statements
            printf("Equation not valid\n");
        }
        //moving the pointer to next point
        listPtr = listPtr->next;
    }
    int numberOfEquations = i;
    int equ = 0;
    //this while loop to move to all equations that were read
    while (equ < numberOfEquations)
    {
        //calculating the array size using strlen function
        int copiedEquationLength = strlen(copiedEquation[equ]);
        int len = copiedEquationLength - 1;
        int k = 0;
        char postfix[SIZE];
        //this for moving to each equation one by one and index by index
        for (int j = 0; j <= len; j++)
        {
            //converting from infix to postfix
            //this if checks if the first index of the equation is number or not by using isdigit() function
            if (isdigit(copiedEquation[equ][j]) && copiedEquation[equ][j + 1])
            {
                //this while checking if the number is one digit or more.
                while (isdigit(copiedEquation[equ][j]) && j <= len)
                {
                    //store the index on an array and increment the size
                    postfix[k++] = copiedEquation[equ][j];
                    j++;
                }
                //store space postfix array
                postfix[k++] = ' ';
            }
            //this if checking the char is opening brackets.
            if (copiedEquation[equ][j] == '(')
            {
                //if it is opening the push the ( to stack
                push(copiedEquation[equ][j]);
            }
                //this if checking the char is closing brackets.
            else if (copiedEquation[equ][j] == ')')
            {
                //keep popping the char from the stack until it match the opening bracket
                while ((c = pop()) != '(')
                {
                    //store the popped char to postfix array
                    postfix[k++] = c;
                    postfix[k++] = ' ';
                }
            }

            else
            {
                //compare the priority Of Operators using priorityOfOperators()
                priorityOfOperators(arrOfStack[top]);
                priorityOfOperators(copiedEquation[equ][j]);
                //comparing priorities of operators which are inside the stack
                while (top != -1 && priorityOfOperators(arrOfStack[top]) >= priorityOfOperators(copiedEquation[equ][j]))
                {
                    postfix[k++] = pop();
                    postfix[k++] = ' ';
                }
                //after comparing pass them to stack using push
                push(copiedEquation[equ][j]);
            }
        }
        //keep pop the char from the stack until it is become empty
        while (top != -1)
        {
            postfix[k++] = pop();
            postfix[k++] = ' ';
        }
        //put null char
        postfix[k] = '\0';
        printf("\n");
        //print the postfix expression
        if(n==1) {
            printf("Postfix Expression for equation [%d] is: ", (equ + 1));
            printPostfix(postfix);
        }
        //calculating the expression tree using evaluateTree(root) function
        if(n==2) {
            fillStackToTree(postfix);
            printf("Expression tree result for equation [%d] is = %d\n",(equ+1), evaluateTree(root));
        }
        //print the postfix expression and expression tree evaluation on the output file
        if(n==3){
            fprintf(outFileName, "Postfix Expression for equation [%d] is = ",(equ+1));
            //printf("Postfix Expression for equation [%d] is: ", (equ + 1));
            printPostfixToFile(postfix, outFileName);
            fillStackToTree(postfix);//calling fillStackToTree(postfix) function
            fprintf(outFileName,"Expression tree result for equation [%d] is = %d\n",(equ+1), evaluateTree(root));
            fprintf(outFileName,"\n");
        }
        equ++;
    }
}
////////////////////////////////////////////////////////////////////////////////////////
//in this function the validity of equations
int checkIsVaild(char expression[])
{
    int count = 0;
    for (int i = 0; expression[i] != '\0'; i++)
    {
        //if it is open bracket search to close bracket
        if (expression[i] == '(')
        {
            count++;
        }
        else if (expression[i] == ')')
        {
            count--;
            if (count < 0)
            {
                return 0;  // More closing parentheses than opening parentheses
            }
        }
    }
    return count == 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int isempty() {
    if (top == -1) {
        return 1;
    }
    return 0;
}*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to remove any space or special char from the equation
void removeSpacesAndSpecialChars(char equation[])
{
    int i, j;

    for (i = 0, j = 0; equation[i] != '\0'; i++)
    {
        if (equation[i] != ' ' && equation[i] != '[' && equation[i] != ']' && equation[i] != '{' &&
            equation[i] != '}')
        {
            equation[j++] = equation[i];
        }
    }
    equation[j] = '\0';
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to build expression tree from postfix expression
void fillStackToTree(char postfix[])
{
    int num = 0, k = 0;
    char myArray[10] = {'\0'};
    char numToString[10] = {'\0'};
    //iterating over the postfix array
    for (int i = 0; postfix[i] != '\0'; i++)
    {
        //this while loop used to find the char for more than one-digit number together
        //in same char array and convert this char array to an integer
        while (postfix[i] != ' ' && !isOpretor(postfix[i]))
        {
            //printf(" char = %c\n", postfix[i]);
            myArray[k++] = postfix[i];
            myArray[k] = '\0';
            //printf("i= %d \n", i);
            i++;
        }
        //this condition contains the conversion part from array of char to int and
        //create an expression tree node and save the number inside this tree node then
        //push this tree node into stackToTree
        if (k > 0)
        {
            k = 0;
            // the atoi was used to convert an array of char into an int
            num = atoi(myArray);
            memset(myArray, '\0', sizeof(myArray));
            //printf("my array = %d\n", num);
            //convert int number into a string to save in a tree node
            sprintf(numToString, "%d", num);
            struct expTreeNode *testTree = newNode(numToString);
            memset(myArray, '\0', sizeof(numToString));
            pushToStructStack(testTree);
        }
        //do the same operation above from creating an exp tree node and push it to stack but in case that the
        //number is one digit
        if (!isOpretor(postfix[i]) && k == 0 && postfix[i] != ' ')
        {
            char charNum[2];
            charNum[0] = postfix[i];
            num = atoi(charNum);
            //printf("my array = %d\n", num);
            sprintf(numToString, "%d", num);
            struct expTreeNode *testTree = newNode(numToString);
            memset(myArray, '\0', sizeof(numToString));
            pushToStructStack(testTree);
        }
        //in case the char is an operation we need to do two pops(left,right) then create tree
        else if (isOpretor(postfix[i]))
        {
            //printf("the operator is %c \n", postfix[i]);
            struct stackNode *left = pop2();
            struct stackNode *right = pop2();

            numToString[0] = postfix[i];
            //makes the root each time points to subtree until the tree became ready so the root points to upper node
            root = createTree(numToString, right, left);
            memset(myArray, '\0', sizeof(numToString));
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function was used to create a subtree each time and push that subtree to the stackToTree  again
struct expTreeNode *createTree(char operator[10], struct stackNode *left, struct stackNode *right)
{
    struct expTreeNode *treeNode = (struct expTreeNode *) malloc(sizeof(struct expTreeNode));
    strcpy(treeNode->opretor, operator);
    treeNode->left = (struct expTreeNode *) left->tree;
    treeNode->right = (struct expTreeNode *) right->tree;
    pushToStructStack(treeNode);
    return treeNode;
}
//////////&////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this function was used to print the postfix expression
void printPostfix(char postfix[])
{
    for (int k = 0; postfix[k] != '\0'; k++)
    {
        printf("%c", postfix[k]);
    }
    printf("\n");
//    fillStackToTree(postfix);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printPostfixToFile(char postfix[], FILE * fileName)
{
    for (int k = 0; postfix[k] != '\0'; k++)
    {
        fprintf(fileName,"%c", postfix[k]);
    }
    fprintf(fileName,"\n");
//    fillStackToTree(postfix);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function create an empty exp tree node so it can be filled in the code above
struct expTreeNode *newNode(char data[10])
{
    struct expTreeNode *node = (struct expTreeNode *) malloc(sizeof(struct expTreeNode));
    strcpy(node->opretor, data);
    node->left = NULL;
    node->right = NULL;
    return node;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function is to evaluate the exp tree by passing the root pointer starting from the lowest subtree until reaching
//the top root by using recursion function
int evaluateTree(struct expTreeNode *root)
{
    // Base case: if the node is a leaf (operand)
    if (root->left == NULL && root->right == NULL)
    {
        return atoi(root->opretor); // Convert operand to integer and return
    }

    //evaluate the left and right subtrees
    int leftValue = evaluateTree(root->left);
    int rightValue = evaluateTree(root->right);

    // Apply the operator at the current node
    if (strcmp(root->opretor, "+") == 0)
    {
        return leftValue + rightValue;
    }
    else if (strcmp(root->opretor, "-") == 0)
    {
        return leftValue - rightValue;
    }
    else if (strcmp(root->opretor, "*") == 0)
    {
        return leftValue * rightValue;
    }
    else if (strcmp(root->opretor, "%") == 0)
    {
        // Assuming non-zero divisor for simplicity
        return leftValue % rightValue;
    }
    else if (strcmp(root->opretor, "/") == 0)
    {
        // Assuming non-zero divisor for simplicity
        return leftValue / rightValue;
    }
    else
    {
        // Handle other operators as needed
        // For now, return 0 for unknown operators
        return 0;
    }
}
/////////////////////////////////////////////////////////////////