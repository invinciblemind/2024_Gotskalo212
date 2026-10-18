#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "lex_analyze.h"
#define SIZE 16
using namespace std;


char * buf; /* буфер для накопления текущего слова*/
int sizebuf; /* размер буфера текущего слова*/
int curbuf, max_curbuf = 0, curlist; /* индекс текущего символа в буфере*/
int sizelist;

char ** lst;

void clearlist(){ // очистить список слов (освободить все указатели на слова и указатель на список)
    int i;
    sizelist=0;
    curlist=0;
    if (lst==NULL) return;
    for (i=0; lst[i]!=NULL; i++)
        free(lst[i]);
    free(lst);
    lst=NULL;
}

void null_list(){
    sizelist=0;
    curlist=0;
    lst=NULL;
}

void termlist(){ // завершение списка и точное выравнивание памяти
    if (lst==NULL) return;
    if (curlist>sizelist-1)
        lst=(char**)realloc(lst,(sizelist+1)*sizeof(*lst));
    lst[curlist]=NULL;
    /*выравниваем используемую под список память точно по размеру списка*/
    lst=(char**)realloc(lst,(sizelist=curlist+1)*sizeof(*lst));
}

void nullbuf(){
    buf=NULL;
    sizebuf=0;
    curbuf=0;
}

void addsym(int c){ // добавление текущего символа к текущей строке
    if (curbuf>sizebuf-1)
        buf=(char*)realloc(buf, sizebuf+=SIZE); /* увеличиваем буфер при необходимости */
    buf[curbuf++]=c;
}

void addword(){ // завершение текущего слова и добавление слова в список (с преобразованием памяти)
    if (curbuf>sizebuf-1)
        buf=(char*)realloc(buf, sizebuf+=1); /* для записи ’\0’ увеличиваем буфер при необходимости */
    buf[curbuf++]='\0';
    if (max_curbuf < curbuf)
        max_curbuf = curbuf;

    /*выравниваем используемую память точно по размеру слова*/
    buf=(char*)realloc(buf,sizebuf=curbuf);
    if (curlist>sizelist-1)
        lst=(char**)realloc(lst, (sizelist+=SIZE)*sizeof(*lst)); /* увеличиваем массив под список при необходимости */
    lst[curlist++]=buf;
}

void printlist(){
    int i;
    if (lst==NULL) return;
    for (i=0; i<sizelist-1; i++) {
        if (lst[i][0] == '\n') {
            cout<<"\\n"<<' '<<i<<endl;
        }
        else {
            if (lst[i][0] == ' ') {
                cout<<"@"<<' '<<i<<endl;
            }
            else {
                cout<<lst[i]<<' '<<i<<endl;
            }
        }
    }
}

char ** ret_lst() {
    return lst;
}

int ret_sizelst() {
    return sizelist;
}

int is_name(char c) {
    return ((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (('0' <= c) && (c <= '9')) || (c == '_')) ? 1 : 0;
}
