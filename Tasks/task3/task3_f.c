#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3.h"
#define SIZE 16


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
        lst=realloc(lst,(sizelist+1)*sizeof(*lst));
    lst[curlist]=NULL;
    /*выравниваем используемую под список память точно по размеру списка*/
    lst=realloc(lst,(sizelist=curlist+1)*sizeof(*lst));
}

void nullbuf(){
    buf=NULL;
    sizebuf=0;
    curbuf=0;
}

void addsym(int c){ // добавление текущего символа к текущей строке
    if (curbuf>sizebuf-1)
        buf=realloc(buf, sizebuf+=SIZE); /* увеличиваем буфер при необходимости */
    buf[curbuf++]=c;
}

void addword(){ // завершение текущего слова и добавление слова в список (с преобразованием памяти)
    if (curbuf>sizebuf-1)
        buf=realloc(buf, sizebuf+=1); /* для записи ’\0’ увеличиваем буфер при необходимости */
    buf[curbuf++]='\0';
    if (max_curbuf < curbuf)
        max_curbuf = curbuf;

    /*выравниваем используемую память точно по размеру слова*/
    buf=realloc(buf,sizebuf=curbuf);
    if (curlist>sizelist-1)
        lst=realloc(lst, (sizelist+=SIZE)*sizeof(*lst)); /* увеличиваем массив под список при необходимости */
    lst[curlist++]=buf;
}

void printlist(){
    int i;
    if (lst==NULL) return;
    for (i=0; i<sizelist-1; i++)
        printf("%s\n",lst[i]);
}

void sortlist() { // сортировка списка (пузырек)
    int i, j;
    char *swapword=NULL;
    for (i = 0; i < sizelist-2; i++)
        for (j = i + 1; j < sizelist-1; j++) {
            if (strcmp(lst[i], lst[j]) > 0) {
                swapword = lst[i];
                lst[i] = lst[j];
                lst[j] = swapword;
            }
        }
}

void del2list() {
    int i;
    for (i = 2; i <= sizelist - 2; i += 2) {
        lst[i / 2]=realloc(lst[i / 2], max_curbuf+1);
        lst[i]=realloc(lst[i], max_curbuf+1);
        strcpy(lst[i / 2], lst[i]);
    }
    for (i = sizelist / 2; i <= sizelist - 2; i++) {
        free(lst[i]);
        lst[i] = NULL;
    }
    sizelist = sizelist / 2 + 1;
    curlist = sizelist - 1;
}

void heshlist() {
    int i, j = 1;
    for (i = 1; i < sizelist - 1; i++) {
        if ((strcmp(lst[i], "##") != 0) || (strcmp(lst[i - 1], "&&") != 0)) {
            if (i != j) {
                lst[j - 1]=realloc(lst[j - 1], max_curbuf+1);
                lst[i - 1]=realloc(lst[i - 1], max_curbuf+1);
                strcpy(lst[j - 1], lst[i - 1]);
            }
            j++;
        }
    }
    if (i != j) {
        lst[j - 1]=realloc(lst[j - 1], max_curbuf+1);
        lst[i - 1]=realloc(lst[i - 1], max_curbuf+1);
        strcpy(lst[j - 1], lst[i - 1]);
    }
    j = i - j;
    for (i = 1; i <= j; i++) {
        free(lst[sizelist - 1 - i]);
        lst[sizelist - 1 - i] = NULL;
    }
    sizelist = sizelist - j;
    curlist = sizelist - j;
}

int ret_sizelist() {
    return sizelist;
}

char ** ret_lst() {
    return lst;
}

int symset(int c){ // проверка на спецсимволы
    return c!='\n' &&
           c!=' '  && 
           c!='\t' && 
           c!='>'  && 
           c!='|'  && 
           c!='&'  && 
           c!=';'  && 
           c!='<'  && 
           c!='('  && 
           c!=')'  && 
           c!='#'  && 
           c!= EOF ;
}
// b c a bc cc ac abc
