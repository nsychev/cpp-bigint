#ifndef BIGINT_VECTOR_H
#define BIGINT_VECTOR_H

#include <cstddef>
#include <memory>
#include <variant>
#include <array>

struct vector {
public:
    typedef uint32_t* iterator;
    typedef const uint32_t* const_iterator;

    vector();
    explicit vector(size_t);
    vector(const vector&);
    vector(const std::initializer_list<uint32_t>& src);
    vector& operator=(const vector&);
    ~vector() = default;

    void resize(size_t, uint32_t);

    void push_back(uint32_t);
    void pop_back();

    const uint32_t& back() const;

    uint32_t& operator[](size_t);
    const uint32_t& operator[](size_t) const;

    size_t size() const;
    bool empty() const;

    iterator begin();
    const_iterator begin() const;

    void clear();
    void insert(const_iterator it, size_t number, uint32_t value);
    void erase(iterator left, iterator right);

    void swap(vector&) noexcept;
private:
    static const size_t SMALL_SIZE = 5;
    typedef std::array<uint32_t, SMALL_SIZE> small_vector;

    size_t _size;

    struct big_vector {
        size_t _capacity = 0;
        std::shared_ptr<uint32_t> _data;

        big_vector(size_t capacity, std::shared_ptr<uint32_t> data):
                _capacity(capacity),
                _data(std::move(data)) {}
    };

    std::variant<small_vector, big_vector> storage;

    void ensure_capacity(size_t size);
    void duplicate();

    inline bool is_small() const;
    inline bool is_big() const;
};

#endif //BIGINT_VECTOR_H
