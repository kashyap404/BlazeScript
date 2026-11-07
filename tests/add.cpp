#include <iostream>
using namespace std;
int add(int a, int b) {
    int sum = a + b;
    return sum; 
}

int main() {
    int x, y;
    
    cout << "Enter two numbers: ";
    cin >> x >> y;
    int result = add(x, y);
    cout << "The sum: " << result << endl;

    return 0;
}
