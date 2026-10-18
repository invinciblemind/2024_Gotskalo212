#include <stdio.h>
#include <stdlib.h>

typedef struct Node *link; /* указатель на звено */
typedef int elemtype; /* тип элемента списка */
typedef struct Node { /* звено состоит из двух полей: */
    elemtype elem; /* элемент списка, */
    link right; /* указатель на следующее звено */
    link left; /* указатель на следующее звено */
} node;
typedef link list; /* список задается указателем на звено */

void add(list *tree, int number) {
    if (*tree == NULL) {
        //printf("thereF\n");
        *tree = (link)malloc(sizeof(node));
        (*tree)->elem = number;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        //printf("%d\n", *tree==NULL);
    }
    else if ((*tree)->elem > number)
        add(&((*tree)->left), number);
    else if ((*tree)->elem < number)
        add(&((*tree)->right), number);
}

void balance(list *tree) {
    link q, w;
    if ((*tree)->left == NULL && (*tree)->right == NULL) {
        free(*tree);
        *tree = NULL;
    }
    else if ((*tree)->right == NULL) {
        q = *tree;
        *tree = (*tree)->left;
        free(q);
    }
    else if ((*tree)->left == NULL) {
        q = *tree;
        *tree = (*tree)->right;
        free(q);
    }
    else {
        q = *tree;
        *tree = (*tree)->left;
        w = q->right;
        while (w->left != NULL)
            w = w->left;
        w->left = (*tree)->right;
        (*tree)->right = q->right;
        free(q);
    }
}

void del(list *tree, int number) {
    if (*tree == NULL);
    else if ((*tree)->elem == number) {
        balance(tree);
    }
    else if ((*tree)->elem > number)
        del(&((*tree)->left), number);
    else if ((*tree)->elem < number)
        del(&((*tree)->right), number);
}

void find(list *tree, int number) {
    if (*tree == NULL)
        printf("no\n");
    else if ((*tree)->elem == number)
        printf("yes\n");
    else if ((*tree)->elem > number)
        find(&((*tree)->left), number);
    else if ((*tree)->elem < number)
        find(&((*tree)->right), number);
}

int deep(list *tree) {
    int left_deep, right_deep;
    if ((*tree) == NULL)
        return 0;
    else {
        left_deep = deep(&((*tree)->left));
        right_deep = deep(&((*tree)->right));
        if (left_deep < right_deep)
            return 1 + right_deep;
        else
            return 1 + left_deep;
    }
}

int main() {
    list tree = NULL;
    int number;
    char c;
    while (1) {
        c = getchar();
        while ((c == ' ' || c == '\t' || c == '\n') && c != EOF)
            c = getchar();
        //printf("there0\n");
        if (c == EOF)
            break;
        scanf("%d", &number);
        if (c == '+') {
            add(&tree, number);
            //printf("%d\n", tree==NULL);
        }
        else if (c == '-') {
            del(&tree, number);
        }
        else if (c == '?') {
            printf("%d ", number);
            find(&tree, number);
        }
        else if (c == 'd') {
            printf("%d\n", deep(&tree));
        }
    }
    return 0;
}
