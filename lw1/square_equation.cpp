#include <cstdio>
#include <cmath>

float calculateLinear(float b, float c)
{
    return -c / b;
}

float calculateRoot(float a, float b, float sqrtDesc)
{
    return (-b + sqrtDesc) / (2 * a);
}

int main ()
{
    std::puts("please enter a, b, c for `ax^2 + bx + c = 0`");

    float a = 0;
    float b = 0;
    float c = 0;
    std::scanf("%f %f %f", &a, &b, &c);

    float desc = (b * b) + (4 * a * c);

    if (a == 0 && b == 0 && c == 0) {
        std::printf("solution: any x");
        return 0;
    } else if (a == 0 && c != 0) {
        std::printf("solution: %f:", calculateLinear(b, c));
        return 0;
    }
    

    if (desc > 0) {
        float sqrtD = std::sqrt(desc);
        float x1 = calculateRoot(a, b, -sqrtD);
        float x2 = calculateRoot(a, b, sqrtD);
        std::printf("solution: %f and %f:", x1, x2);
    } else if (desc == 0) {
        std::printf("solution: %f:", (-b / (2 * a)));
    } else {
        std::printf("no solution");
    }
}