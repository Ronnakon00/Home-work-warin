#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct Node {
    char name[50];
    int score;
    struct Node* next;
    struct Node* prev;
} Node;

Node* createNode(char* name, int score) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->score = score;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addScore(Node** head, char* name, int score) {
    Node* newNode = createNode(name, score);
    
    if (*head == NULL) {
        *head = newNode;
    } 
    else if (score >= (*head)->score) {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    } 
    else {
        Node* curr = *head;
        while (curr->next != NULL && curr->next->score > score) {
            curr = curr->next;
        }
        
        newNode->next = curr->next;
        if (curr->next != NULL) {
            curr->next->prev = newNode;
        }
        curr->next = newNode;
        newNode->prev = curr;
    }

    int count = 1;
    Node* temp = *head;
    while (temp->next != NULL) {
        count++;
        if (count > MAX_SIZE) {
            Node* last = temp->next;
            temp->next = NULL;
            free(last);
            break;
        }
        temp = temp->next;
    }
}

void displayBoard(Node* head) {
    printf("\n--- SCORE BOARD ---\n");
    Node* curr = head;
    int rank = 1;
    while (curr != NULL) {
        printf("%d. %-10s %d\n", rank++, curr->name, curr->score);
        curr = curr->next;
    }
    if (head == NULL) printf("Board is empty.\n");
}

int main() {
    Node* board = NULL;

    addScore(&board, "Ryu", 100);
    addScore(&board, "Ken", 98);
    addScore(&board, "Chunli", 95);
    addScore(&board, "Sagat", 94);

    displayBoard(board);

    printf("\nUpdate score board");
    addScore(&board, "Vega", 97);

    displayBoard(board);

    return 0;
}