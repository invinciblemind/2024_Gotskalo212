#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void oct(int number)
{
    if (number < 8)
        printf("%d", number);
    else {
        oct(number / 8);
        printf("%d", number % 8);
    }
}

int main(int argc, char **argv)
{
    int count = 1, c1, i, flag;
    FILE *f1;
    flag = !(strcmp(argv[1], "-b"));
    if ((f1=fopen(argv[flag + 1], "r")) == NULL) {
        fprintf(stderr, "ошибка при открытии файла 1\n");
        return 1;
    }
    c1=fgetc(f1);
    while (c1 != EOF) {
        
        if (count % 10 == 1) {
            printf("\n");
            for (i = 10000000; i > 0; i = i / 10)
                printf("%d", (count / i) % 10);
            printf("   ");
        }
        if (c1 == 13) {
            count++;
            c1 = fgetc(f1);
            if (c1 == 10)
                if (flag)
                    printf("015 012 ");
                else
                    printf("\\n");
            else {
                if (count % 10 == 1) {
                    printf("\n");
                    for (i = 10000000; i > 0; i = i / 10)
                        printf("%d", (count / i) % 10);
                    printf("   ");
                }
                if (flag) {
                    printf("015 ");
                    oct(c1);
                    printf(" ");
                }
                else
                    printf("%c%c", (char)13, (char)c1);
            }
        }
        else
            if (flag) {
                if (c1 <= 63)
                    printf("0");
                oct(c1);
                printf(" ");
            }
            else
                printf("%c", (char)c1);
        count++;
        c1=fgetc(f1);
    }
    printf("\n");
    return 0;
}
