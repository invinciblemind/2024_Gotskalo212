#include <stdio.h>
#include <unistd.h>
#define N 1024

int main(int argc, char **argv)
{
    char s[N];
    if (getcwd(s, N) != NULL)
        printf("%s\n", s);
    else
        return 1;
    return 0;
}
