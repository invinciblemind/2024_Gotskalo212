#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i;
    if (strcmp(argv[1], "-n") != 0) {
        for (i = 1; i <= argc - 1; i++)
            printf("%s ", argv[i]);
        printf("\n");
    }
    else {
        for (i = 2; i <= argc - 1; i++)
            printf("%s ", argv[i]);
    }
}
