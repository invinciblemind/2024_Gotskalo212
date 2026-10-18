#include <iostream>
#include <cstdlib>
#include "lex_analyze.h"

int c;
FILE *f;

int getsym() {
    return fgetc(f);
}

char **lex_analyzer(char *file_name) {
    f = fopen(file_name, "r");
    typedef enum {Start, Word, IsWithEqual, Mult, Div, Quot, Quot2, DoubleQuot, DoubleQuot2, FinishWord} vertex; // реализация графа
    vertex V=Start;
    null_list();
    c = getsym();
    
    while(1) 
        switch(V){ // моделирование работы графа
            case Start:
                if (c == EOF) { // завершение списка лексем и его возврат
                    termlist();
                    fclose(f);
                    return ret_lst();
                }
                else {
                    nullbuf();
                    if (c != 13) // особый случай: \n - это 2 символа с кодами 10 и 13, и чтобы в список лексем попадал только один символ, другой мы пропускаем
                        addsym(c);
                    else {
                        c = getsym();
                        break;
                    }
                    switch(c){
                        case '>': case '<': case '!': case '+': case '-': case '=': // переход в доп. состояние для проверки, есть ли после этого символа знак =
                            V = IsWithEqual;
                            break;
                        case '*': // доп. состояние для проверки есть ли далее * или =
                            V = Mult;
                            break;
                        case '/': // доп. состояние для проверки есть ли далее / или =
                            V = Div;
                            break;
                        case '\'': // доп. состояние для проверки есть ли далее '
                            V = Quot;
                            break;
                        case '"': // доп. состояние для проверки есть ли далее ""
                            V = DoubleQuot;
                            break;
                        default: // иначе - на состояние завершения слова
                            V = FinishWord;
                    }
                    if (is_name(c)) { // если же символ - часть имени, то переход в состояние Слово
                        V = Word;
                    }
                    c = getsym();
                }
                break;

            case Word:
                if (is_name(c)) {
                    addsym(c);
                    c = getsym();
                }
                else {
                    V = FinishWord;
                }
                break;
            
            case IsWithEqual:
                if (c == '=') {
                    addsym(c);
                    c = getsym();
                }
                V = FinishWord;
                break;
            
            case Mult:
                if (c == '=' || c == '*') {
                    addsym(c);
                    c = getsym();
                }
                V = FinishWord;
                break;
            
            case Div:
                V = FinishWord;
                if (c == '=') {
                    addsym(c);
                    c = getsym();
                }
                else if (c == '/') {
                    addsym(c);
                    c = getsym();
                    V = IsWithEqual;
                }
                break;
            
            case Quot:
                V = FinishWord;
                if (c == '\'') {
                    addsym(c);
                    c = getsym();
                    V = Quot2;
                }
                break;
            
            case Quot2:
                V = FinishWord;
                if (c == '\'') {
                    addsym(c);
                    c = getsym();
                }
                else { // здесь мы имеем две кавычки - пустая строка, но т. к. в буфере уже 2 кавычки, то создаем пустой комментарий, а уже потом добавляем в список раздельно 2 кавычки
                    addsym('\'');
                    addword();
                    nullbuf();
                    addsym('\'');
                    addsym('\'');
                    addsym('\'');
                    addword();
                    nullbuf();
                    addsym('\'');
                    addword();
                    nullbuf();
                    addsym('\'');
                }
                break;
            
            case DoubleQuot:
                V = FinishWord;
                if (c == '"') {
                    addsym(c);
                    c = getsym();
                    V = DoubleQuot2;
                }
                break;
            
            case DoubleQuot2:
                V = FinishWord;
                if (c == '"') {
                    addsym(c);
                    c = getsym();
                }
                else { // аналогично как с одинарной кавычкой
                    addsym('"');
                    addword();
                    nullbuf();
                    addsym('"');
                    addsym('"');
                    addsym('"');
                    addword();
                    nullbuf();
                    addsym('"');
                    addword();
                    nullbuf();
                    addsym('"');
                }
                break;
            
            
            case FinishWord:
                V = Start;
                addword();
                break;

        } // конец switch
}
