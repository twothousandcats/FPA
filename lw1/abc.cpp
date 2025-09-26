#include <cstdio>

int calculate(int a, int b, int c) {
    return a + b + c;
}

int main()
{
    int a = 0;
    int b = 0;
    int c = 0;

    std::scanf("%d %d %d", &a, &b, &c);
    std::printf("%d", calculate(a, b, c));
}