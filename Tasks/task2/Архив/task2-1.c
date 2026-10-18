#include <stdio.h>

double square_i, square_i1, number, eps;
char c;

int main() {
    scanf("%lf", &eps);
    while (1) {
        scanf("%lf", &number);
        square_i = 1.0;
        while (1) {
            square_i1 = (square_i + number / square_i) / 2;
            if ((square_i - square_i1) * (square_i > square_i1?1:-1) < eps)
                break;
            square_i = square_i1;
        }
        c = getchar();
        if (c == EOF) break;
        printf("%.10g\n", square_i1);
    }
}
