#include <iostream>
#include <deque>
#include <malloc.h>

template <typename T>
class Vector {
public:

    T * data = nullptr;
    int _capacity;
    int _size;

    Vector() : _capacity(0), _size(0) {}

    Vector(int n) : _capacity(n), _size(n) {
        data = static_cast<T*>(::operator new(sizeof(T) * n));
    }

    Vector(Vector& other) : _capacity(other._size), _size(other._size) {
        data = malloc(sizeof(T) * _capacity);
        for (int i = 0; i < _size; i++) {
            data[i] = other.data[i];
        }
    }

    void push_back(T& element) {
        if (_capacity == _size) reallocate();
        new (data + sizeof(T)*(_size++)) T(element);
    }

    void push_back(T&& element) {
        if (_capacity == _size) reallocate();
        new (data + sizeof(T)*(_size++)) T(element);
    }

    void pop_back() {
        _size--;
    }

    void reallocate() {
        _capacity <<= 1;
        if (_capacity == 0) _capacity = 1;
        T * new_data = static_cast<T*>(::operator new(sizeof(T) * _capacity));
        for (int i = 0; i < _size; i++) {
            new (new_data + sizeof(T) * i) T(data[i]);
        }
        std::destroy(data, data + _size);
        ::operator delete(data);
        data = new_data;
    }

    int size() const {
        return _size;
    }

    int capacity() const {
        return _capacity;
    }

    T& operator[](int pos) {
        return data[pos];
    }

    const T& operator[](int pos) const {
        return data[pos];
    }

    Vector& operator=(const Vector& other) {
        std::destroy(data, data + _capacity);
        ::operator delete(data);
        _capacity = other._capacity;
        _size = other._size;

        data = static_cast<T*>(::operator new(sizeof(T) * _capacity));
        for (int i = 0; i < _size; i++) {
            new (data + sizeof(T) * i) T(other[i]);
        }
        return *this;
    }

    void reserve(int _sz) {
        if (_sz > _capacity) {
            _capacity = _sz + 100;
            T * newData = static_cast<T*>(::operator new(sizeof(T) * _capacity));
            for (int i = 0; i < _size; i++) {
                new (newData + sizeof(T)*i) T(data[i]);
            }
            std::destroy(data, data + _size);
            ::operator delete(data);
            data = newData;
        }
    }

    void resize(int _sz) {
        if (_sz < _size) {
            std::destroy(data + _sz, data + _size);
            _size = _sz;
        } else {
            for (int i = _size; i < _sz; i++) {
                push_back(T());
            }
        }
    }

    void clear() {
        std::destroy(data, data + _size);
        _size = 0;
    }

    T * begin() {
        return data;
    }

    T * end() {
        return (data + _size);
    }

    ~Vector() {
        std::destroy(data, data + _size);
        ::operator delete(data);
    }
};
