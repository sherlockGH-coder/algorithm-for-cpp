#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

class BigDouble {
private:
    std::string integer;
    std::string decimal;
    bool negative;

    void normalize();
    static std::pair<BigDouble, BigDouble> align(const BigDouble& a, const BigDouble& b);

public:
    BigDouble(const std::string& num);
    BigDouble(double num);
    BigDouble() : integer("0"), decimal("0"), negative(false) {}
    BigDouble(int num) : BigDouble(std::to_string(num)) {}

    // 加法
    BigDouble operator+(const BigDouble& other) const;
    // 减法
    BigDouble operator-(const BigDouble& other) const;
    // 乘法
    BigDouble operator*(const BigDouble& other) const;
    // 除法
    BigDouble operator/(const BigDouble& other) const;
    // 求幂
    BigDouble pow(int exponent) const;
    // 负号
    BigDouble operator-() const;
    
    // 比较大小
    bool operator>=(const BigDouble& other) const;

    bool operator!=(const BigDouble& other) const;

    // 重载cout
    friend std::ostream& operator<<(std::ostream& os, const BigDouble& num);

    std::string toString() const;
};

BigDouble::BigDouble(const std::string& num) {
    size_t dotPos = num.find('.');
    negative = num[0] == '-';
    if (dotPos == std::string::npos) {
        integer = num.substr(negative ? 1 : 0);
        decimal = "0";
    } else {
        integer = num.substr(negative ? 1 : 0, dotPos - (negative ? 1 : 0));
        decimal = num.substr(dotPos + 1);
    }
    normalize();
}

BigDouble::BigDouble(double num) : BigDouble(std::to_string(num)) {}

void BigDouble::normalize() {
    // 移除整数部分的前导0
    integer.erase(0, std::min(integer.find_first_not_of('0'), integer.size() - 1));
    // 移除小数部分的后导0
    decimal.erase(decimal.find_last_not_of('0') + 1);
    if (decimal.empty()) decimal = "0";
}

// 对齐两个数的小数部分
std::pair<BigDouble, BigDouble> BigDouble::align(const BigDouble& a, const BigDouble& b) {
    BigDouble a_copy = a, b_copy = b;
    size_t max_decimal = std::max(a.decimal.length(), b.decimal.length());
    a_copy.decimal.resize(max_decimal, '0');
    b_copy.decimal.resize(max_decimal, '0');
    return {a_copy, b_copy};
}

BigDouble BigDouble::operator+(const BigDouble& other) const {
    // 对齐小数部分
    std::pair<BigDouble, BigDouble> aligned = align(*this, other);
    const BigDouble& a = aligned.first;
    const BigDouble& b = aligned.second;

    std::string result_int, result_dec;
    // 每次相加的向前一位进的数
    int carry = 0;

    // 小数部分相加
    for (int i = a.decimal.length() - 1; i >= 0; --i) {
        int sum = (a.decimal[i] - '0') + (b.decimal[i] - '0') + carry;
        result_dec = std::to_string(sum % 10) + result_dec;
        carry = sum / 10;
    }

    for (int i = a.integer.length() - 1, j = b.integer.length() - 1; i >= 0 || j >= 0 || carry; --i, --j) {
        int sum = carry;
        if (i >= 0) sum += a.integer[i] - '0';
        if (j >= 0) sum += b.integer[j] - '0';
        result_int = std::to_string(sum % 10) + result_int;
        carry = sum / 10;
    }

    BigDouble result(result_int + "." + result_dec);
    result.negative = negative;
    return result;
}

BigDouble BigDouble::operator-(const BigDouble& other) const {
    auto aligned = align(*this, other);
    const BigDouble& a = aligned.first;
    const BigDouble& b = aligned.second;

    if (a.integer.length() < b.integer.length() ||
        (a.integer.length() == b.integer.length() && a.integer < b.integer)) {
        return -(b - a);
    }

    std::string result_int, result_dec;
    int borrow = 0;

    for (int i = a.decimal.length() - 1; i >= 0; --i) {
        int diff = (a.decimal[i] - '0') - (b.decimal[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result_dec = std::to_string(diff) + result_dec;
    }

    for (int i = a.integer.length() - 1, j = b.integer.length() - 1; i >= 0; --i, --j) {
        int diff = (a.integer[i] - '0') - (j >= 0 ? b.integer[j] - '0' : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result_int = std::to_string(diff) + result_int;
    }

    BigDouble result(result_int + "." + result_dec);
    result.negative = negative;
    return result;
}

BigDouble BigDouble::operator*(const BigDouble& other) const {
    std::vector<int> result(integer.length() + decimal.length() + other.integer.length() + other.decimal.length(), 0);
    
    auto multiply = [&](const std::string& a, const std::string& b) {
        for (int i = a.length() - 1; i >= 0; --i) {
            for (int j = b.length() - 1; j >= 0; --j) {
                int mul = (a[i] - '0') * (b[j] - '0');
                int sum = result[i + j + 1] + mul;
                result[i + j + 1] = sum % 10;
                result[i + j] += sum / 10;
            }
        }
    };

    multiply(integer + decimal, other.integer + other.decimal);

    std::string result_str;
    bool leading_zero = true;
    for (int digit : result) {
        if (digit != 0 || !leading_zero) {
            result_str += std::to_string(digit);
            leading_zero = false;
        }
    }
    if (result_str.empty()) result_str = "0";

    int decimal_points = decimal.length() + other.decimal.length();
    if (decimal_points >= result_str.length()) {
        result_str = "0." + std::string(decimal_points - result_str.length(), '0') + result_str;
    } else {
        result_str.insert(result_str.length() - decimal_points, ".");
    }

    BigDouble result_big(result_str);
    result_big.negative = negative ^ other.negative;
    return result_big;
}

BigDouble BigDouble::operator/(const BigDouble& other) const {
    if (other.integer == "0" && other.decimal == "0") {
        throw std::invalid_argument("Division by zero");
    }

    BigDouble dividend = *this;
    BigDouble divisor = other;

    bool result_negative = negative ^ other.negative;

    std::cout << "dividend: " << dividend.toString() << std::endl;
    std::cout << "divisor: " << divisor.toString() << std::endl;

    std::string result_int, result_dec;

    // 对齐小数部分
    auto aligned = align(dividend, divisor);
    dividend = aligned.first;
    divisor = aligned.second;

    // 精度
    int precision = 10;
    

    std::string result;
    BigDouble temp("0");

    std::string dividend_num = dividend.integer + dividend.decimal;
    std::string divisor_num = divisor.integer + divisor.decimal;


    for (int i = 0; result_dec.length() != precision; i++) {
        if (i >= dividend_num.length()) {
            temp = temp * BigDouble("10");
        } else {
            char digit = dividend_num[i];
            temp = temp * BigDouble("10") + BigDouble(std::string(1, digit));
        }
        
        if (temp >= divisor_num) {
            int quotient = 0;
            while (temp >= divisor_num) {
                temp = temp - divisor_num;
                quotient ++;
            }
            if (result.length() == dividend_num.length()) result_dec += std::to_string(quotient);
            else result += std::to_string(quotient);
        } else {
            if (result.length() == dividend_num.length()) result_dec += "0";
            else result += "0";
        }
    }

    // 去除result的前导0
    size_t cnt2 = 0;
    while (result[cnt2] == '0' && result.length() - 1 > cnt2) cnt2 ++;
    result_int = result.substr(cnt2);

    // 去除result_dec的后导0
    size_t cnt3 = result_dec.length() - 1;
    while (result_dec[cnt3] == '0' && cnt3 > 0) cnt3 --;
    result_dec = result_dec.substr(0, cnt3 + 1);

    result = (result_negative ? "-" : "") + result_int + "." + result_dec;
    return BigDouble(result);
}

BigDouble BigDouble::pow(int exponent) const {
    if (exponent == 0) return BigDouble("1");
    if (exponent < 0) {
        return BigDouble("1") / pow(-exponent);
    }
    BigDouble result("1");
    BigDouble base = *this;
    while (exponent > 0) {
        if (exponent & 1) result = result * base;
        base = base * base;
        exponent >>= 1;
    }
    return result;
}

BigDouble BigDouble::operator-() const {
    BigDouble result = *this;
    result.negative = !result.negative;
    return result;
}

// 比较大小
bool BigDouble::operator>=(const BigDouble& other) const {
    if (negative != other.negative) {
        return other.negative;
    }

    auto aligned = align(*this, other);
    const BigDouble& a = aligned.first;
    const BigDouble& b = aligned.second;

    if (a.integer.length() != b.integer.length()) {
        return (a.integer.length() > b.integer.length()) ^ negative;
    }

    if (a.integer != b.integer) {
        return (a.integer > b.integer) ^ negative;
    }

    return (a.decimal >= b.decimal) ^ negative;
}

bool BigDouble::operator!= (const BigDouble& other) const {
    return integer != other.integer || decimal != other.decimal || negative != other.negative;
}

std::string BigDouble::toString() const {
    std::string result = (negative ? "-" : "") + integer;
    if (decimal != "0") {
        result += "." + decimal;
    }
    return result;
}

std::ostream& operator<<(std::ostream& out, const BigDouble& num) {
    out << num.toString();
    return out;
}

void test() {
    BigDouble a = 123134.214;
    BigDouble b = 2134.12;
    BigDouble c = a + b;
    BigDouble d = a - b;
    BigDouble e = a * b;
    BigDouble f = a / b;
    BigDouble g = a.pow(3);
    std::cout << a << "+" << b << "=" << c << std::endl;
    std::cout << a << "-" << b << "=" << d << std::endl;
    std::cout << a << "*" << b << "=" << e << std::endl;
    std::cout << a << "/" << b << "=" << f << std::endl;
    std::cout << a << "^3=" << g << std::endl;
}

int main() {
    test();
    return 0;
}