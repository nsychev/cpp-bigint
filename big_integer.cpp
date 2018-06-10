#include "big_integer.h"

#include <string>
#include "vector.h"
#include <stdexcept>

big_integer::big_integer(): digits(), sign(false) {
}

big_integer::big_integer(big_integer const& other): digits(other.digits), sign(other.sign) {
}

big_integer::big_integer(int a): digits(1), sign(a < 0) {
    digits[0] = static_cast<unsigned>(a);
    shrink_to_fit();
}

big_integer::big_integer(std::string const& str) {
    this->digits = {0};
    this->sign = false;

    bool negate = false;
    size_t pos = 0;

    if (str[pos] == '-') {
        negate = true;
        pos++;
    }

    while (pos < str.length()) {
        if (str[pos] < '0' || str[pos] > '9') {
            throw std::runtime_error(std::string("unexpected char '") + str[pos] + "' in given string");
        }
        *this = this->mul_uint(10);
        *this += int(str[pos] - '0');

        pos++;
    }

    if (negate)
        *this = -*this;
}

big_integer::~big_integer() = default;

big_integer& big_integer::operator=(big_integer const& other)
{
    digits = other.digits;
    sign = other.sign;
    return *this;
}
