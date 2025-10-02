#include <iostream>

using namespace std;

#define max 100

int main() {
    // цикл выводит числа от 1 до 100
    for (int min = 1; min <= max; min++) {
        if (min % 3 == 0 && min % 5 == 0) {
            cout << "FizzBuzz" << endl;
        } else if (min % 3 == 0) {
            cout << "Fizz" << endl;
        } else if (min % 5 == 0) {
            cout << "Buzz" << endl;
        } else {
            cout << min << endl;
        }
    }

    return 0;
}
