#include <stdio.h>

double x, polynomial = 0, a_i, differential = 0;
char c;

int main() {
    scanf("%lf", &x);
    while (1) {
        scanf("%lf", &a_i);
        c = getchar();
        if (c == EOF) break;
        differential = differential * x + polynomial;
        polynomial = polynomial * x + a_i;
    }
    printf("%.10g\n%.10g\n", polynomial, differential);
}
