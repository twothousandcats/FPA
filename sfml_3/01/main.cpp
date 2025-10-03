#include <iostream>

using namespace std;

int main() {
    for (int num = 1; num <= 100; num++) {
        switch (num % 15) {
            case 0:
                cout << "FizzBuzz" << endl;
                break;
            case 5:
            case 10:
                cout << "Buzz" << endl;
                break;
            case 3:
            case 6:
            case 9:
            case 12:
                cout << "Fizz" << endl;
                break;
            default:
                cout << num << endl;
        }
    }
}
