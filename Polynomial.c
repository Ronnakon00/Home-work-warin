#include <stdio.h>
#include <stdlib.h>

struct Poly {
    struct Poly *Nodef; 
    int digit_B, digit_P; 
    struct Poly *Next;
};

struct Poly *create_Node(int digit_B, int digit_P) {
    struct Poly *new_node = (struct Poly*)malloc(sizeof(struct Poly));
    new_node->digit_B = digit_B;
    new_node->digit_P = digit_P;
    new_node->Nodef = NULL;
    new_node->Next = NULL;
    return new_node;
}

void append(struct Poly **Head, int digit_B, int digit_P) {
    if (digit_B == 0) return;
    if (*Head == NULL) {
        *Head = create_Node(digit_B, digit_P);
        return;
    }
    struct Poly *temp = *Head;
    if (digit_P > temp->digit_P) {
        struct Poly *new_Node = create_Node(digit_B, digit_P);
        new_Node->Next = temp;
        temp->Nodef = new_Node;
        *Head = new_Node;
        return;
    }
    while (temp != NULL) {
        if (temp->digit_P == digit_P) {
            temp->digit_B += digit_B;
            return;
        }
        if (temp->Next == NULL || temp->Next->digit_P < digit_P) {
            struct Poly *new_Node = create_Node(digit_B, digit_P);
            new_Node->Next = temp->Next;
            new_Node->Nodef = temp;
            if (temp->Next != NULL) temp->Next->Nodef = new_Node;
            temp->Next = new_Node;
            return;
        }
        temp = temp->Next;
    }
}

void append_end(struct Poly **Head, struct Poly **Last, int b, int p) {
    if (b == 0) return;
    struct Poly *new_node = create_Node(b, p);
    if (*Head == NULL) {
        *Head = new_node;
        *Last = new_node;
    } else {
        (*Last)->Next = new_node;
        new_node->Nodef = *Last;
        *Last = new_node;
    }
}

struct Poly* add_poly(struct Poly *P1, struct Poly *P2) {
    struct Poly *resHead = NULL;
    struct Poly *resLast = NULL;
    struct Poly *t1 = P1;
    struct Poly *t2 = P2;

    while (t1 != NULL || t2 != NULL) {
        if (t1 != NULL && (t2 == NULL || t1->digit_P > t2->digit_P)) {
            append_end(&resHead, &resLast, t1->digit_B, t1->digit_P);
            t1 = t1->Next;
        } 
        else if (t2 != NULL && (t1 == NULL || t2->digit_P > t1->digit_P)) {
            append_end(&resHead, &resLast, t2->digit_B, t2->digit_P);
            t2 = t2->Next;
        } 
        else {
            int sum_B = t1->digit_B + t2->digit_B;
            if (sum_B != 0) {
                append_end(&resHead, &resLast, sum_B, t1->digit_P);
            }
            t1 = t1->Next;
            t2 = t2->Next;
        }
    }
    return resHead;
}

struct Poly* multiply_poly(struct Poly *P1, struct Poly *P2) {
    struct Poly *res = NULL;
    for (struct Poly *i = P1; i != NULL; i = i->Next) {
        for (struct Poly *j = P2; j != NULL; j = j->Next) {
            append(&res, i->digit_B * j->digit_B, i->digit_P + j->digit_P);
        }
    }
    return res;
}

void show(struct Poly *Head) {
    if (Head == NULL) { printf("0\n"); return; }
    struct Poly *temp = Head;
    int first = 1;
    while (temp != NULL) {
        if (temp->digit_B == 0) { temp = temp->Next; continue; }
        if (!first && temp->digit_B > 0) printf("+");
        if (temp->digit_P == 0) printf("%d", temp->digit_B);
        else if (temp->digit_P == 1) printf("%dX", temp->digit_B);
        else printf("%dX^%d", temp->digit_B, temp->digit_P);
        first = 0;
        temp = temp->Next;
    }
    if (first) printf("0");
    printf("\n");
}

int main() {
    int n, b, p;
    printf("Amount of polynomial : ");
    if(scanf("%d", &n) != 1) return 0;
    struct Poly *heads[n];

    for (int i = 0; i < n; i++) {
        heads[i] = NULL;
        printf("Push poly %d (Format: 4+3X^3+2X^1-4X^2 00):\n", i + 1);
        while (1) {
            if (scanf("%d", &b) != 1) break; 
            char c = getchar();
            if (c == 'X' || c == 'x') {
                char check = getchar();
                if (check == '^') scanf("%d", &p);
                else { p = 1; ungetc(check, stdin); } 
            } else {
                p = 0; ungetc(c, stdin);
            }
            if (b == 0 && p == 0) break;
            append(&heads[i], b, p);
        }
    }

    printf("\n--- Results ---\n");
    for(int i=0; i<n; i++) { printf("Poly %d: ", i+1); show(heads[i]); }
    
    if (n >= 2) {
        struct Poly *sum = add_poly(heads[0], heads[1]);
        printf("Addition (1+2): "); show(sum);
        struct Poly *multi = multiply_poly(heads[0], heads[1]);
        printf("Multiplication (1*2): "); show(multi);
    }
    return 0;
}