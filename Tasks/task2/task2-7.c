#include <stdio.h>
#include <setjmp.h>

jmp_buf begin;
char curlex;
void getlex(void);
int expr(void);
int add_sub(void);
int mult_del(void);
int degree(void);
void error();

int main()
{
    int result;
    setjmp(begin);
    printf("==>");
    getlex();
    result = expr();
    if (curlex == EOF);
    else if (curlex != '\n')
        error();
    else {
        printf("\n%d\n", result);
        longjmp(begin, 1);
    }
    return 0;
}

void getlex()
{
    while ( (curlex=getchar()) == ' ');
}

void error(void)
{
    char c;
    printf("\nОШИБКА!\n");
    c = getchar();
    while(c != '\n' && c != EOF)
        c = getchar();
    if (c != EOF)
        longjmp(begin, 1);
}

int expr()
{
    int e = add_sub();
    //printf("expr = %d\n", e);
    while (curlex == '+' || curlex == '-') {
        if (curlex == '+') {
            getlex();
            e += add_sub();
        }
        else {
            getlex();
            e -= add_sub();
        }
        //printf(" expr = %d\n", e);
    }
    return e;
}

int add_sub()
{
    int b, a = mult_del();
    //printf("add_sub = %d\n", a);
    while (curlex == '*' || curlex == '/') {
        if (curlex == '*') {
            getlex();
            a *= mult_del();
        }
        else {
            getlex();
            b = mult_del();
            if (b == 0) {
                error();
                longjmp(begin, 1);
            }
            a /= b;
        }
        //printf(" add_sub = %d\n", a);
    }
    return a;
}

int mult_del()
{
    int a_old, a = degree(), b;
    //printf("mult_del = %d\n", a);
    if (curlex == '^')
    {
        getlex();
        b = mult_del();
        a_old = a;
        a = 1;
        while (b-- > 0)
            a *= a_old;
        //printf(" mult_del = %d\n", a);
    }
    return a;
}

int degree()
{
    int m = 1;
    switch(curlex){
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
                m = curlex - '0';
                break;
        case '(':
            getlex();
            m = expr();
            if (curlex == ')')
                break;
        case EOF:
            break;
        default :
            error();
    }
    getlex();
    //printf("degree = %d\n", m);
    return m;
}
