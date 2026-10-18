#include <stdio.h>

double x, answer = 0, a_i;
char c;

int main() {
    scanf("%lf", &x);
    while (1) {
        scanf("%lf", &a_i);
        c = getchar();
        if (c == EOF) break;
        answer = answer * x + a_i;
    }
    printf("%.10g\n", answer);
}
