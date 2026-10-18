#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int c1, c2;
    int line = 1, chr = 1;
    FILE *f1, *f2;
    if ((f1=fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "ошибка при открытии файла 1\n");
        return 1;
    }
    if ((f2=fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "ошибка при открытии файла 2\n");
        return 1;
    }
    c1=fgetc(f1);
    c2=fgetc(f2);
    while (c1 != EOF && c2 != EOF) {
        if (c1 != c2)
            break;
        if (c1 == '\n') {
            line++;
        }
        chr++;
        c1=fgetc(f1);
        c2=fgetc(f2);
    }
    if (c1 != EOF || c2 != EOF)
        printf("%s differs from %s: line %d, char %d\n", argv[1], argv[2], line, chr);
    else
        printf("Files are the same\n");
    return 0;
}
