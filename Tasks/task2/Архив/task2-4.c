#include <stdio.h>

char c;
char str[100];

double str2double(char str[]) {
    char *str_pointer = str;
    int i;
    double number = 0, coef = 0.1, degree = 0, coefR = 10;
    if (*str_pointer == '-') {
        coefR = 0.1;
        str_pointer++;
    }
    else if (*str_pointer == '+') str_pointer++;
    while (*str_pointer != '.' && *str_pointer != 'e' &&
     *str_pointer != 'E' && *str_pointer != 'R' && *str_pointer != 'r') {
        number = number * 10 + *str_pointer - '0';
        str_pointer++;
    }
    if (*str_pointer == 'r' || *str_pointer == 'R') {
        degree = number;
        number = 0;
        str_pointer++;
        while (*str_pointer != '.' && *str_pointer != '\0' &&
         *str_pointer != 'L' && *str_pointer != 'l' &&
         *str_pointer != 'F' && *str_pointer != 'f') {
            number = number * 10 + *str_pointer - '0';
            str_pointer++;
        }
        if (*str_pointer == '.') {
            str_pointer++;
            while (*str_pointer != 'L' && *str_pointer != 'l' &&
             *str_pointer != 'F' && *str_pointer != 'f' &&
             *str_pointer != '\0') {
                number = number + (*str_pointer - '0') * coef;
                coef /= 10;
                str_pointer++;
            }
        }
        for (i = 1; i <= degree; i++) {
            number *= coefR;
        }
        return number;
    }
    else if (*str_pointer == '.') {
        str_pointer++;
        while (*str_pointer != 'L' && *str_pointer != 'l' &&
         *str_pointer != 'F' && *str_pointer != 'f' &&
         *str_pointer != 'e' && *str_pointer != 'E' &&
         *str_pointer != '\0') {
            number = number + (*str_pointer - '0') * coef;
            coef /= 10;
            str_pointer++;
        }
        if (*str_pointer == '\0')
            return number;
    }
    // сейчас *str_pointer на е/Е
    coef = 10;
    str_pointer++;
    if (*str_pointer == '-') {
        coef = 0.1;
        str_pointer++;
    }
    else if (*str_pointer == '+') str_pointer++;
    while (*str_pointer != 'L' && *str_pointer != 'l' &&
     *str_pointer != 'F' && *str_pointer != 'f' &&
     *str_pointer != '\0') {
            degree = degree * 10 + *str_pointer - '0';
            str_pointer++;
        }
    for (i = 1; i <= degree; i++) {
        number *= coef;
    }
    return number;
}

int main() {
    while (1) {
        scanf("%s", &str);
        c = getchar();
        if (c == EOF) break;
        printf("%.10g\n", str2double(str));
    }
}
// int i; while(scanf("%d", &i)==1){}
