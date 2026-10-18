#include <stdio.h>
#include <stdlib.h>
#include "task3.h"
#define N 5

int cursym = 0;

char *str;

char c;

char getsym() // ввод символов блоками длины N
{
    char sym;
    if (str[cursym] == '\0') {
        cursym = 0;
        if (fgets(str, N, stdin) == NULL)
            return EOF;
    }
    sym = str[cursym];
    cursym++;
    return sym;
}

int main()
{
    typedef enum {Start, Word, Greater, Stick, And, SingleWord, Newline, Stop, Hesh} vertex; // реализация графа
    vertex V=Start;
    null_list();
    
    str = (char *)calloc(N, sizeof(char));
    str[0] = '\0';
    printf("\nПервая строка: ");
    c = getsym();
    
    while(1==1) 
        switch(V){ // моделирование работы графа
            case Start:
                if(c==' '||c=='\t')
                    c=getsym();
                else if (c==EOF) {
                    if (ret_lst() != NULL) {
                        termlist();
                        printf("Количество слов: %d\nСписок слов:\n", ret_sizelist()-1);
                        printlist();
                        
                        printf("\nС опцией #:\n");
                        heshlist();
                        printlist();
                        
                        printf("\nОтсортированный список слов:\n");
                        sortlist();
                        printlist();
                        printf("\nБез элементов на четных позициях:\n");
                        del2list();
                        printlist();
                        printf("\nНовая строка: ");
                    }
                    clearlist();
                    V=Stop;
                }
                else if (c=='\n') {
                    if (ret_lst() != NULL) {
                        termlist();
                        printf("Количество слов: %d\nСписок слов:\n", ret_sizelist()-1);
                        printlist();
                        
                        printf("\nС опцией #:\n");
                        heshlist();
                        printlist();
                        
                        printf("\nОтсортированный список слов:\n");
                        sortlist();
                        printlist();
                        printf("\nБез элементов на четных позициях:\n");
                        del2list();
                        printlist();
                        printf("\nНовая строка: ");
                    }
                    V=Newline;
                    c=getsym();
                }
                else {
                    nullbuf();
                    addsym(c);
                    switch(c){
                        case '>':
                            V = Greater;
                            break;
                        case '|':
                            V = Stick;
                            break;
                        case '&':
                            V = And;
                            break;
                        case '#':
                            V = Hesh;
                            break;
                        case ';': case '<': case '(': case ')':
                            V = SingleWord;
                            break;
                        default:
                            V = Word;
                    }
                    c=getsym();
                }
                break;

            case Word:
                if(symset(c)) {
                    addsym(c);
                    c=getsym();
                }
                else {
                    V=Start;
                    addword();
                }
                break;

            case Greater:
                if(c=='>') {
                    addsym(c);
                    c=getsym();
                    V=SingleWord;
                }
                else {
                    V=Start;
                    addword();
                }
                break;
            
            case Hesh:
                if(c=='#') {
                    addsym(c);
                    c=getsym();
                    V=SingleWord;
                }
                else {
                    V=Word;
                }
                break;

            case Stick:
                if(c=='|') {
                    addsym(c);
                    c=getsym();
                    V=SingleWord;
                }
                else {
                    V=Start;
                    addword();
                }
                break;
            
            case And:
                if(c=='&') {
                    addsym(c);
                    c=getsym();
                    V=SingleWord;
                }
                else {
                    V=Start;
                    addword();
                }
                break;
            
            case SingleWord:
                V=Start;
                addword();
                break;
            
            case Newline:
                clearlist();
                V=Start;
                break;

            case Stop:
            free(str);
                exit(0);
                break;
        } // конец switch
}
