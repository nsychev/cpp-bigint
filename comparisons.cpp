#include "big_integer.h"

bool big_integer::is_zero() const {
    return !sign && digits.size() == 0;
}

bool operator==(big_integer const& a, big_integer const& b) {
    big_integer d = a - b;
    return d.is_zero();
}

bool operator<(big_integer const& a, big_integer const& b) {
    big_integer d = a - b;
    return d.sign;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator>(big_integer const& a, big_integer const& b) {
    return b < a;
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return !(b < a);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}
