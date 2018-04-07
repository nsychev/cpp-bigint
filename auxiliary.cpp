#include "big_integer.h"
#include <string>
#include <algorithm>
#include <tuple>
#include <stdexcept>

big_integer abs(big_integer const& a)  {
    if (a < 0)
        return -a;
    return a;
}

void big_integer::shrink_to_fit() {
    while (digits.size() > 0 && digits.back() == static_cast<unsigned>(-sign))
        digits.pop_back();
}

void big_integer::ensure_size(size_t capacity) {
    if (digits.size() > capacity)
        return;
    digits.resize(capacity, static_cast<unsigned>(-sign));
}

std::string to_string(big_integer const& a) {
    if (a.is_zero())
        return "0";

    std::string res;

    big_integer val = abs(a);
    int digit;

    while (!val.is_zero()) {
        std::tie(val, digit) = val.div_uint(10);
        res += static_cast<char>(digit + '0');
    }

    if (a < 0)
        res += "-";

    std::reverse(res.begin(), res.end());

    return res;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}
