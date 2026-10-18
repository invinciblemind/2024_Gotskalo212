#include <stdio.h>

double x, polynomial = 0, a_i, differential = 0, integral = 0;
int i, n;
char c;

int main() {
    scanf("%d", &n);
    scanf("%lf", &x);
    for (i = 0; i <= n; i++) {
        scanf("%lf", &a_i);
        differential = differential * x + polynomial;
        polynomial = polynomial * x + a_i;
        integral = integral * x + a_i * x / (double)(n + 1 - i);
    }
    printf("%.10g\n%.10g\n%.10g\n", polynomial, differential, integral);
}
