#include "big_integer.h"
#include <stdexcept>
#include <cmath>
#include <tuple>

std::pair<big_integer, unsigned> big_integer::div_uint(unsigned rhs) const {
    big_integer result;
    result.ensure_size(this->digits.size());

    size_t i = this->digits.size(), tmp = 0;
    while (i--) {
        tmp <<= 32;
        tmp += this->digits[i];
        result.digits[i] = static_cast<unsigned>(tmp / rhs);
        tmp = tmp % rhs;
    }

    result.shrink_to_fit();

    return {result, tmp};
}

big_integer big_integer::mul_uint(unsigned rhs) const {
    big_integer result = abs(*this);

    result.ensure_size(digits.size() + 1);
    unsigned long long tmp = 0;

    for (size_t i = 0; i < result.digits.size(); i++) {
        tmp += 1ULL * result.digits[i] * rhs;
        result.digits[i] = static_cast<unsigned>(tmp & UINT32_MAX);
        tmp >>= 32;
    }

    result.shrink_to_fit();

    if (sign)
        result = -result;

    return result;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer r = ~*this + 1;
    return r;
}

big_integer& big_integer::operator++() {
    return *this = *this + 1;
}

const big_integer big_integer::operator++(int) {
    big_integer r = *this;
    *this += 1;
    return r;
}

big_integer& big_integer::operator--() {
    return *this = *this - 1;
}

const big_integer big_integer::operator--(int) {
    big_integer r = *this;
    *this -= 1;
    return r;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    ensure_size(std::max(digits.size(), rhs.digits.size()) + 1);
    unsigned long long tmp = 0;
    for (size_t i = 0; i < digits.size(); i++) {
        tmp += digits[i];

        if (i < rhs.digits.size())
            tmp += rhs.digits[i];
        else
            tmp += static_cast<unsigned>(-rhs.sign);

        digits[i] = static_cast<unsigned>(tmp & UINT32_MAX);
        tmp >>= 32;
    }
    sign = bool(digits.back() >> 31);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    ensure_size(std::max(digits.size(), rhs.digits.size()) + 1);
    unsigned long long tmp, sub = 0;
    for (size_t i = 0; i < digits.size(); i++) {
        tmp = digits[i];

        if (i < rhs.digits.size())
            sub += rhs.digits[i];
        else
            sub += static_cast<unsigned>(-rhs.sign);

        if (tmp >= sub) {
            tmp -= sub;
            sub = 0;
        } else {
            tmp = UINT32_MAX + 1 + tmp - sub;
            sub = 1;
        }

        digits[i] = static_cast<unsigned>(tmp);
    }
    sign = bool(digits.back() >> 31);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    bool result_sign = sign ^ rhs.sign;
    big_integer right = abs(rhs);
    *this = abs(*this);

    big_integer result;
    for (size_t i = 0; i < right.digits.size(); i++) {
        result += this->mul_uint(right.digits[i]) << (32 * i);
    }

    if (result_sign)
        result = -result;

    return *this = result;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    const unsigned long long BASE = 1ULL << 32;

    if (rhs.is_zero()) {
        throw std::runtime_error("division by zero");
    }

    bool result_sign = sign ^ rhs.sign;
    big_integer right = abs(rhs);
    big_integer &left = *this;
    left = abs(left);

    if (left.digits.size() < right.digits.size()) {
        return left = 0;
    }

    if (right.digits.size() == 1) {
        std::tie(left, std::ignore) = left.div_uint(right.digits[0]);

        if (result_sign)
            left = -left;

        return left;
    }

    size_t n = right.digits.size(), m = left.digits.size() - n;
    unsigned long long d = BASE / (right.digits.back() + 1);
    left = left.mul_uint(static_cast<unsigned>(d));
    right = right.mul_uint(static_cast<unsigned>(d));
    m++;

    big_integer result;
    result.ensure_size(m);

    unsigned divisor = right.digits.back();
    while (m--) {
        left.ensure_size(m + n + 1);
        unsigned long long dividend = ((1ULL * left.digits[n + m]) << 32) + left.digits[n + m - 1];
        unsigned long long quotient = dividend / divisor;
        unsigned long long remainder = dividend % divisor;

        if (quotient >= BASE)
            quotient = BASE - 1;

        while (remainder < BASE &&
               (quotient == BASE || quotient * right.digits[n - 2] > BASE * remainder + left.digits[m + n - 2])) {
            quotient--;
            remainder += divisor;
        }

        if ((left >> (32 * m)) < right.mul_uint(static_cast<unsigned>(quotient)))
            quotient--;

        left -= right.mul_uint(static_cast<unsigned>(quotient)) << (32 * m);

        result.digits[m] = static_cast<unsigned>(quotient);
    }

    if (result_sign)
        result = -result;
    return *this = result;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    return *this = *this - (*this / rhs) * rhs;
}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}
