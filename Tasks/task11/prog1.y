%{
#include <stdio.h>

int yylex(void);
void yyerror(char *);
%}
%%
P: S{printf("Протяжение = %d\n", $1 );}
S: /* верхний уровень скобок */ '(' Q ')' S{$$ = $2 + $4;} | /* eps */ {$$ = 0;}
Q: S{$$ = 1;} /* то, что внутри скобок 1-го уровня, всегда возвращает 1 */
%%
int main(){
    printf("Введите скобочную систему: ");
    yyparse();
}

int yylex(){
    int c;
    c = getchar();
    if (c == '\n')
	return 0;
    yylval = c;
    return c;
}

void yyerror(char *s) {
    printf("Ошибка: %s\n", s);
}