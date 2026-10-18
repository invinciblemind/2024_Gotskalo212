#include <stdio.h>
#include <stdlib.h>
#include "task3.h"
#define N 5

int cursym = 0;

char *str;

int c, cavychka_flag = 0;

int getsym() // ввод символов блоками длины N
{
    int sym;
    if (str[cursym] == '\0') {
        cursym = 0;
        if (fgets(str, N, stdin) == NULL)
            return EOF;
    }
    sym = str[cursym];
    cursym++;
    return sym;
}

char **task3()
{
    typedef enum {Start, Word, Greater, Stick, And, SingleWord, Newline, Stop, Ignore} vertex; // реализация графа
    vertex V=Start;
    null_list();
    
    str = (char *)calloc(N, sizeof(char));
    str[0] = '\0';
    printf("\n$=> ");
    c = getsym();
    
    while(1==1) 
        switch(V){ // моделирование работы графа
            case Start:
                if(c==' '||c=='\t')
                    c=getsym();
                else if (c==EOF) {
                    if (ret_lst() != NULL) {
                        termlist();
                        free(str);
                        return ret_lst();
                    }
                    clearlist();
                    V=Stop;
                }
                else if (c=='\n') {
                    if (ret_lst() != NULL) {
                        termlist();
                        free(str);
                        return ret_lst();
                    }
                    V=Newline;
                    c=getsym();
                }
                else {
                    nullbuf();
                    if (c != '\\' && c != '#')
                        addsym(c);
                    switch(c){
                        case '\\':
                            V = Ignore;
                            break;
                        case '"':
                            cavychka_flag = 1;
                            V = Word;
                            break;
                        case '#':
                            while (c != '\n' && c != EOF)
                                c = getsym();
                            break;
                        case '>':
                            if (cavychka_flag == 1)
                                break;
                            V = Greater;
                            break;
                        case '|':
                            if (cavychka_flag == 1)
                                break;
                            V = Stick;
                            break;
                        case '&':
                            if (cavychka_flag == 1)
                                break;
                            V = And;
                            break;
                        case ';': case '<': case '(': case ')':
                            if (cavychka_flag == 1)
                                break;
                            V = SingleWord;
                            break;
                        default:
                            V = Word;
                    }
                    if (c != '\n' && c != EOF)
                        c=getsym();
                }
                break;

            case Word:
                if (c == '\\') {
                    V = Ignore;
                    c = getsym();
                }
                else if (c == '"') {
                    if (cavychka_flag == 0) {
                        addword();
                        nullbuf();
                        addsym(c);
                        c=getsym();
                        cavychka_flag = 1;
                    }
                    else {
                        cavychka_flag = 0;
                        addsym(c);
                        V=Start;
                        addword();
                        c=getsym();
                    }
                }
                else if (c == '#') {
                    addword();
                    while (c != '\n' && c != EOF)
                        c = getsym();
                    V = Start;
                    break;
                }
                else if (c == EOF) {
                    V=Start;
                    addword();
                }
                else if(symset(c) || cavychka_flag == 1) {
                    addsym(c);
                    c=getsym();
                }
                else {
                    V=Start;
                    addword();
                }
                break;
            
            case Ignore:
                addsym(c);
                c=getsym();
                V = Word;
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
                printf("\n");
                free(str);
                exit(0);
                break;
        } // конец switch
}
