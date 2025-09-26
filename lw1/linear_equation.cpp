#include <cstdio>

float calculate(float a, float b)
{
    return -b / a;
}

int main ()
{
    std::puts("please enter a and b for `ax + b = 0`");

    float a = 0;
    float b = 0;
    std::scanf("%f %f", &a, &b);
    std::printf("solution: %f:", calculate(a, b));
}