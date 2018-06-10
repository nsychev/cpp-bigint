#include "big_integer.h"

big_integer& big_integer::operator&=(big_integer const& rhs) {
    ensure_size(std::max(digits.size(), rhs.digits.size()) + 1);
    for (size_t i = 0; i < digits.size(); i++) {
        if (i < rhs.digits.size())
            digits[i] &= rhs.digits[i];
        else
            digits[i] &= static_cast<unsigned>(-rhs.sign);
    }
    sign = bool(digits.back() >> 31);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
    ensure_size(std::max(digits.size(), rhs.digits.size()) + 1);
    for (size_t i = 0; i < digits.size(); i++) {
        if (i < rhs.digits.size())
            digits[i] |= rhs.digits[i];
        else
            digits[i] |= static_cast<unsigned>(-rhs.sign);
    }
    sign = bool(digits.back() >> 31);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
    ensure_size(std::max(digits.size(), rhs.digits.size()) + 1);
    for (size_t i = 0; i < digits.size(); i++) {
        if (i < rhs.digits.size())
            digits[i] ^= rhs.digits[i];
        else
            digits[i] ^= static_cast<unsigned>(-rhs.sign);
    }
    sign = bool(digits.back() >> 31);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator<<=(int rhs) {
    digits.insert(digits.begin(), (rhs + 31u) / 32u, 0);
    rhs = (32 - rhs % 32) % 32;
    return *this >>= rhs;
}

big_integer& big_integer::operator>>=(int rhs) {
    size_t count = rhs / 32U;
    if (digits.size() <= count) {
        digits.clear();
        sign = false;
    }
    digits.erase(digits.begin(), digits.begin() + count);
    rhs %= 32;
    if (rhs == 0)
        return *this;

    ensure_size(digits.size() + 1);

    for (size_t i = 0; i < digits.size() - 1; i++) {
        digits[i] = (digits[i] >> rhs) | (digits[i + 1] << (32 - rhs));
    }

    shrink_to_fit();

    return *this;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

big_integer big_integer::operator~() const {
    big_integer r = *this;
    for (size_t i = 0; i < digits.size(); i++) {
        r.digits[i] = ~r.digits[i];
    }
    r.sign = !r.sign;
    r.shrink_to_fit();
    return r;
}
