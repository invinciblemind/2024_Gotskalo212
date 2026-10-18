#include <stdio.h>
#include <stdlib.h>

typedef struct Node *link; /* указатель на звено */
typedef char elemtype; /* тип элемента списка */
typedef struct Node { /* звено состоит из двух полей: */
    elemtype symbol; /* элемент списка, */
    link next_symbol; /* указатель на следующее звено */
} node;
typedef link list; /* список задается указателем на звено */

typedef struct NodeL *linkL; /* указатель на звено */
typedef struct NodeL { /* звено состоит из двух полей: */
    list word; /* элемент списка, */
    linkL next_word; /* указатель на следующее звено */
} nodeL;
typedef linkL listL; /* список задается указателем на звено */
listL list_of_words;

void print( list p)
{
    while( p!= NULL) { /* пока не конец списка, */
        putchar( p->symbol) ; /* печатаем очередной элемент */
        p = p->next_symbol ; /* и переходим к следующему звену */
    }
    putchar( '\n') ;
}

void destruct( list ls)
{
    link q;
    while ( ls != NULL ) {
        q = ls ;
        ls = ls->next_symbol ;
        free(q) ;
    }
} 

int main() {
    char c = ' ';
    link new_word, last_word, last_word_now;
    linkL word_pointer, word_pointer_last;
    list_of_words = NULL;
    while (c != '\n') {
        while ((c == ' ' || c == '\t') && c != '\n')
            c = getchar();
        if (c != '\n') {
            if (list_of_words == NULL)
                list_of_words = word_pointer = (linkL)malloc(sizeof(nodeL));
            else
                word_pointer = word_pointer->next_word = (linkL)malloc(sizeof(nodeL));
            word_pointer->word = last_word = new_word = (link)malloc(sizeof(node));
            new_word->symbol = c;
            c = getchar();
            while (c != ' ' && c != '\t' && c != '\n') {
                new_word = new_word->next_symbol = (link)malloc(sizeof(node));
                new_word->symbol = c;
                c = getchar();
            }
            new_word->next_symbol = NULL;
            word_pointer->next_word = NULL;
        }
    }
    
    /* печать всех элементов
    word_pointer = list_of_words;
    while (word_pointer != NULL) {
        print(word_pointer->word);
        word_pointer = word_pointer->next_word;
    }
    */
    
    word_pointer = word_pointer_last = list_of_words;
    if (word_pointer == NULL)
        return 0;
    int flag_first = 1;
    //printf("there1\n");
    while (word_pointer->next_word != NULL) {
        //printf("thereI*\n");
        new_word = word_pointer->word;
        last_word_now = last_word;
        //print(new_word);
        //print(last_word_now);
        //printf("\n");
        while (new_word->symbol == last_word_now->symbol) {
            //printf("thereC\n");
            new_word = new_word->next_symbol;
            last_word_now = last_word_now->next_symbol;
            //printf("thereC*\n");
            if (new_word == NULL || last_word_now == NULL)
                break;
        }
        //printf("thereN\n");
        if (new_word == NULL && last_word_now == NULL) // удалить слово
            if (flag_first) {
                //printf("there1D\n");
                new_word = word_pointer->word;
                destruct(new_word);
                list_of_words = word_pointer->next_word;
                free(word_pointer);
                word_pointer = word_pointer_last = list_of_words;
                //printf("there1D*\n");
                continue;
            }
            else {
                //printf("thereD\n");
                new_word = word_pointer->word;
                destruct(new_word);
                word_pointer_last->next_word = word_pointer_last->next_word->next_word;
                free(word_pointer);
                word_pointer = word_pointer_last;
                //printf("thereD*\n");
            }
        else
            flag_first = 0;
        word_pointer_last = word_pointer;
        //printf("%d\n", word_pointer->next_word == NULL);
        word_pointer = word_pointer->next_word;
        //printf("thereF\n");
    }
    
    word_pointer = list_of_words;
    while (word_pointer != NULL) {
        print(word_pointer->word);
        word_pointer = word_pointer->next_word;
    }
    
    return 0;
}
// abcd ab cd ab abc ab
