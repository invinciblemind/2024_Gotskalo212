#include <stdio.h>

char c;
int i;

int Fibonacci_iter(int i) {
    int fib_0 = 0, fib_1 = 1, i_local = 1, change;
    if (i == 0) return fib_0;
    while (i_local < i) {
        change = fib_1;
        fib_1 = fib_0 + fib_1;
        fib_0 = change;
        i_local++;
    }
    return fib_1;
}

int Fibonacci_rec(int i) {
    if (i == 0) return 0;
    else if (i == 1) return 1;
    return Fibonacci_rec(i - 1) + Fibonacci_rec(i - 2);
}

int main() {
    while (1) {
        scanf("%d", &i);
        c = getchar();
        if (c == EOF) break;
        printf("%d\n%d\n", Fibonacci_iter(i), Fibonacci_rec(i));
    }
}
