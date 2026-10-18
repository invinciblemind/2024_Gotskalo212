#include <stdio.h>

int i, j;

int abs(int x)
{
    return x >= 0 ? x:-x;
}

int main() {
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++) {
            printf("%d, ", i);
            printf("%d  ", j);
            if (i == 0 || j == 0 || (i/abs(i) + j/abs(j) != 0 && 10 / i + 10 / j + 40 / (i + j) == 40))
                if (i * j == 1)
                    printf("Попали сюда, т.к. 10 / i + 10 / j + 40 / (i + j) == 40\n");
                else
                    printf("Попали сюда, т.к. i == 0 || j == 0\n");
            else if (i == -j)
                printf("Попали сюда, т.к. i/abs(i) + j/abs(j) != 0\n");
            else
                printf("Попали сюда, т.к. 10 / i + 10 / j + 40 / (i + j) != 40\n");
        }
}
