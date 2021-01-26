/*************************************
* Lab 1 Exercise 2
* Name     : Daryl Tan
* Matric No: A0132812R
* Lab Group: B17
*************************************/
/*************************************
* Lab 1 Exercise 2
* Name     : Leow Wen Bin
* Matric No: A0184415E
* Lab Group: B13
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE {
    int data;
    struct NODE *next;
} node;

//Function Prototypes
node *insertAt(node *, int, int, int);

void printList(node *);

void destroyList(node *);


int main() {
    node *myList = NULL;    //Empty List
    int position, copies, newValue;

    //Fill in code for input and processing

    while (scanf("%i%i%i", &position, &newValue, &copies) == 3) {
        myList = insertAt(myList, position, copies, newValue);
    }

    //Output code coded for you
    printf("My List:\n");
    printList(myList);

    destroyList(myList);
    myList = NULL;


    printf("My List After Destroy:\n");
    printList(myList);

    return 0;
}

//Actual Function Implementations
node *insertAt(node *head, int position, int copies, int newValue) {
    //Fill in your code here
    node *newHeadPointer = head;
    node *prevNode = NULL;
    node *nodeToInsertBefore = head;
    for (int j = 0; j < position && nodeToInsertBefore != NULL; j++) {
        prevNode = nodeToInsertBefore;
        nodeToInsertBefore = nodeToInsertBefore->next;
    }
    for (int i = 0; i < copies; i++) {
        node *newNode = (node *) malloc(sizeof(node));
        newNode->data = newValue;
        newNode->next = nodeToInsertBefore;
        if (prevNode == NULL) {
            newHeadPointer = newNode;
        } else {
            prevNode->next = newNode;
        }
        nodeToInsertBefore = newNode;
    }
    return newHeadPointer;
}

void printList(node *head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node *ptr = head;

    while (ptr != NULL) {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node *head) {
    //Fill in your code here
    node *ptr = head;

    while (ptr != NULL) {    //or you can write while(ptr)
        node *next = ptr->next;
        free(ptr);
        ptr = next;
    }
}
