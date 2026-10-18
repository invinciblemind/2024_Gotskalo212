#ifndef __LEX
#define __LEX

void clearlist();

void null_list();

void termlist();

void nullbuf();

void addsym(int c);

void addword();

void printlist();

char ** ret_lst();

char **lex_analyzer(char *file_name);

int ret_sizelst();

int is_name(char c);

#endif
