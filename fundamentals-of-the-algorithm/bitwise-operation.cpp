#include <iostream>
#include <climits>
using namespace std;

/*

位运算加法：
通过异或运算的性质可以看出，异或运算相当于不进位的加法，
而进位数则由与运算并左移一位得到。
因此，可以通过递归的方式，将进位数与不进位数相加，
直到进位数为0。

*/
int add(int a, int b) {
    return !b ? a : add(a ^ b, (a & b) << 1);
}

/*

位运算减法：
减法可以通过加法来实现
a - b = a + (-b)
-b = ~b + 1

*/
int sub(int a, int b) {
    return add(a, add(~b, 1));
}

/*

位运算乘法：

*/
int mul_v1(int a, int b) {
    int a1 = a < 0 ? add(~a, 1) : a;
    int b1 = b < 0 ? add(~b, 1) : b;
    int res = 0;
    while (b1) {
        if (b1 & 1) {
            res = add(res, a1);
        }
        a1 <<= 1;
        b1 >>= 1;
    }
    if ((a ^ b) < 0) {
        res = add(~res, 1);
    }
    return res;
}

int mul_v2(int a, int b) {
    int res = 0;
    int a1 = a < 0 ? add(~a, 1) : a;
    int b1 = b < 0 ? add(~b, 1) : b;
    for (int i = 0; i < b1; i++) {
        res = add(a1, res);
    }
    if ((a ^ b) < 0) {
        res = add(~res, 1);
    }
    return res;
}

/*

位运算除法：

*/
int divide(int a, int b) {
    if (b == 0) {
        throw invalid_argument("Division by zero");
    }

    if (a == INT_MIN && b == -1) {
        return INT_MAX; // Handle overflow
    }

    bool negative = (a < 0) ^ (b < 0);
    unsigned int a1 = a < 0 ? static_cast<unsigned int>(-(a + 1)) + 1 : static_cast<unsigned int>(a);
    unsigned int b1 = b < 0 ? static_cast<unsigned int>(-(b + 1)) + 1 : static_cast<unsigned int>(b);
    unsigned int res = 0;

    for (int i = 31; i >= 0; i--) {
        if ((a1 >> i) >= b1) {
            res += 1U << i;
            a1 -= b1 << i;
        }
    }

    return negative ? -static_cast<int>(res) : static_cast<int>(res);
}

/*

取余运算：

*/
int mod(int a, int b) {
    return sub(a, mul_v2(divide(a, b), b));
}

int main() {
    int a = INT_MIN, b = 1;
    cout << add(a, b) << endl;
    cout << sub(a, b) << endl;
    cout << mul_v1(a, b) << endl;
    cout << mul_v2(a, b) << endl;
    cout << "divide(a, b) = " << divide(a, b) << endl;
    cout << mod(a, b) << endl;
    cout << "a / b = " << (a / b) << endl;
    return 0;
}