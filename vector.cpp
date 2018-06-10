#include "vector.h"

#include <algorithm>
#include <variant>
#include <cassert>

vector::vector() : vector(0) {}

vector::vector(size_t size) : _size(size) {
    if (size <= SMALL_SIZE) {
        storage = small_vector();
    } else {
        storage = big_vector(size, std::shared_ptr<uint32_t>(
                new uint32_t[size], std::default_delete<uint32_t[]>()
        ));
    }

    std::fill(begin(), begin() + size, 0);
}

vector::vector(const vector& other): _size(other._size) {
    if (other.is_small()) {
        storage = std::get<small_vector>(other.storage);
        return;
    }

    const big_vector& vec = std::get<big_vector>(other.storage);

    storage = big_vector(vec._capacity, vec._data);
}

vector::vector(const std::initializer_list<uint32_t>& src): vector(src.size()) {
    std::copy(src.begin(), src.end(), begin());
}

vector& vector::operator=(const vector& other) {
    _size = other._size;

    if (other.is_small()) {
        storage = std::get<small_vector>(other.storage);
        return *this;
    }

    const big_vector& vec = std::get<big_vector>(other.storage);
    storage = big_vector(vec._capacity, vec._data);

    return *this;
}

void vector::resize(size_t size, uint32_t value = uint32_t()) {
    duplicate();

    _size = std::min(size, _size);
    ensure_capacity(size);

    std::fill(begin() + _size, begin() + size, value);

    _size = size;
}

void vector::push_back(uint32_t value) {
    duplicate();

    ensure_capacity(_size + 1);
    (*this)[_size] = value;
    _size++;
}

void vector::pop_back() {
    duplicate();

    _size--;
}

const uint32_t& vector::back() const {
    return (*this)[_size - 1];
}

uint32_t& vector::operator[](size_t pos) {
    duplicate();
    return *(begin() + pos);
}

const uint32_t &vector::operator[](size_t pos) const {
    return *(begin() + pos);
}

size_t vector::size() const {
    return _size;
}

bool vector::empty() const {
    return !_size;
}

vector::iterator vector::begin() {
    if (is_big())
        return std::get<big_vector>(storage)._data.get();

    return const_cast<iterator>(std::get<small_vector>(storage).begin());
}

vector::const_iterator vector::begin() const {
    if (is_big())
        return std::get<big_vector>(storage)._data.get();

    return const_cast<const_iterator>(std::get<small_vector>(storage).begin());
}

void vector::clear() {
    _size = 0;
}

void vector::insert(const_iterator it, size_t number, uint32_t value) {
    duplicate();

    assert(it == begin());
    size_t pos = it - begin();
    assert(pos == 0);
    ensure_capacity(_size + number);

    std::copy_backward(begin() + pos, begin() + _size, begin() + _size + number);
    std::fill(begin() + pos, begin() + pos + number, value);

    _size += number;
}

void vector::erase(iterator left, iterator right) {
    duplicate();

    std::copy(right, begin() + _size, left);
    _size -= right - left;
}

void vector::swap(vector& other) noexcept {
    std::swap(_size, other._size);
    storage.swap(other.storage);
}

void vector::ensure_capacity(size_t size) {
    if (is_small()) {
        if (size <= SMALL_SIZE)
            return;
    } else {
        if (size <= std::get<big_vector>(storage)._capacity)
            return;
    }

    std::shared_ptr<uint32_t> data(new uint32_t[size * 2], std::default_delete<uint32_t[]>());
    std::copy(begin(), begin() + _size, data.get());

    storage = big_vector(size * 2, data);
}

void vector::duplicate() {
    if (is_small() || std::get<big_vector>(storage)._data.unique())
        return;

    big_vector& vec = std::get<big_vector>(storage);
    std::shared_ptr<uint32_t> tmp(new uint32_t[vec._capacity], std::default_delete<uint32_t[]>());
    std::copy(vec._data.get(), vec._data.get() + _size, tmp.get());
    vec._data = tmp;
}

inline bool vector::is_small() const {
    return !storage.index();
}

inline bool vector::is_big() const {
    return storage.index();
}
