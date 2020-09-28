#include <iostream>
#include <deque>
#include <memory>
#include <malloc.h>

template <typename T>
class Vector {
public:

    T * data = nullptr;
    int _capacity;
    int _size;

    Vector() : _capacity(0), _size(0) {}

    Vector(int n) : _capacity(n), _size(n) {
        int done = 0;
        try {
            data = (T *) malloc(_capacity * sizeof(T));
            for (int i = 0; i < _size; i++) {
                new (data + i) T();
                done++;
            }
        } catch(...) {
            _size = 0, _capacity = 0;
            Free(data, done);
            data = nullptr;
            throw;
        }
    }

    Vector(Vector& other) : _capacity(other._size), _size(other._size) {
        int done = 0;
        try {
            data = (T *) malloc(_capacity * sizeof(T));
            for (int i = 0; i < _size; i++) {
                new (data + i) T(other.data[i]);
                done++;
            }
        } catch(...) {
            _size = 0, _capacity = 0;
            Free(data, done);
            data = nullptr;
            throw;
        }
    }

    void push_back(const T& element) {
        int oldCapacity = _capacity;
        bool done = false;
        try {
            if (_size == 0 && _capacity != 1) {
                if (_capacity >= 2) free(data + 1);
                    else data = (T*)malloc(sizeof(T) * 1);
                _capacity = 1;
            }
            if (_capacity == _size) reallocate();
            if (_capacity == _size) throw std::exception();
            new (data + _size) T(element);
            done = true;
            _size++;
        } catch(...) {
            if (done) data[_size].~T();
            _capacity = oldCapacity;
            throw;
        }
    }

    void push_back(const T&& element) {
        int oldCapacity = _capacity;
        bool done = false;
        try {
            if (_size == 0 && _capacity != 1) {
                if (_capacity >= 2) free(data + 1);
                    else data = (T*)malloc(sizeof(T) * 1);
                _capacity = 1;
            }
            if (_capacity == _size) reallocate();
            if (_capacity == _size) throw std::exception();
            new (data + _size) T(std::move(element));
            done = true;
            _size++;
        } catch(...) {
            if (done) data[_size].~T();
            _capacity = oldCapacity;
            throw;
        }
    }

    void pop_back() {
        _size--;
        data[_size].~T();
    }

    //void reallocate(int newCapacity) {
    //    int oldCapacity = _capacity, done = 0;
    //    T* ptr = nullptr;
    //    try {
    //        _capacity = newCapacity;
    //        T* newData = (T*)malloc(_capacity * sizeof(T));
    //    ptr = newData;
    //        for (int i = 0; i < _size; i++) {
    //            new (newData + i) T();
    //            done++;
    //            newData[i] = data[i];
    //        }
    //        Free(data, _size);
    //        data = newData;
    //    } catch(...) {
    //        Free(ptr, done);
    //       _capacity = oldCapacity;
    //       throw;
    //  }
    // }
    void reallocate() {
        int oldCapacity = _capacity, done = 0;
        T* ptr = nullptr;
        try {
            _capacity <<= 1;
            if (_capacity == 0) _capacity = 1;
            T* newData = (T*)malloc(_capacity * sizeof(T));
            ptr = newData;
            for (int i = 0; i < _size; i++) {
                new (newData + i) T(data[i]);
                done++;
            }
            Free(data, _size);
            data = newData;
        } catch(...) {
            Free(ptr, done);
            _capacity = oldCapacity;
            throw;
        }
    }

    int size() const {
        return _size;
    }

    int capacity() const {
        return _capacity;
    }

    T& operator[](const int& pos) {
        return data[pos];
    }

    const T& operator[](const int& pos) const {
        return data[pos];
    }

    Vector& operator=(const Vector& other) {
        int oldCapacity = _capacity, oldSize = _size, done = 0;
        T* ptr = nullptr;
        try {
            _capacity = other._size;
            _size = other._size;
            T* newData = (T*) malloc(_capacity * sizeof(T));
            ptr = newData;
            for (int i = 0; i < _size; i++) {
                new (newData + i) T(other.data[i]);
                done++;
            }
            Free(data, oldSize);
            data = newData;
        } catch(...) {
            Free(ptr, done);
            _capacity = oldCapacity;
            _size = oldSize;
            throw;
        }
        return *this;
    }

    void reserve(int _sz) {
        if (_sz > _capacity) {
            int oldCapacity = _capacity, done = 0;
            T* ptr = nullptr;
            try {
                _capacity = _sz;
                T* newData = (T*)malloc(_capacity * sizeof(T));
                ptr = newData;
                for (int i = 0; i < _size; i++) {
                    new (newData + i) T(data[i]);
                    done++;
                }
                Free(data, _size);
                data = newData;
            } catch(...) {
                Free(ptr, done);
                _capacity = oldCapacity;
                throw;
            }
        }
    }

    void swap(Vector<T>& other) {
        std::swap(this->data, other.data);
        std::swap(this->_size, other._size);
        std::swap(this->_capacity, other._capacity);
    }

    void resize(int _sz) {
        int oldSize = _size, oldCapacity = _capacity, done = 0;
        T* ptr = nullptr;
        if (_sz <= _size) {
            for (int i = _sz; i < _size; i++) {
                data[i].~T();
            }
            _size = _sz;
        } else {
            if (_sz <= _capacity) {
                try {
                    for (int i = _size; i < _sz; i++) {
                        new (data + i) T();
                        done++;
                    }
                    _size = _sz;
                } catch(...) {
                    for (int i = _size; done; i++, done--) {
                        data[i].~T();
                    }
                    _size = oldSize;
                    throw;
                }
            } else {
                try {
                    _capacity = _sz;
                    T *newData = (T *) malloc(_capacity * sizeof(T));
                    ptr = newData;
                    for (int i = 0; i < _size; i++) {
                        new (newData + i) T(data[i]);
                        done++;
                    }
                    for (int i = _size; i < _sz; i++) {
                        new (newData + i) T();
                        done++;
                    }
                    Free(data, _size);
                    _size = _sz;
                    data = newData;
                } catch (...) {
                    Free(ptr, done);
                    _capacity = oldCapacity;
                    _size = oldSize;
                    throw;
                }
            }
        }
    }

    void clear() {
        for (int i = 0; i < _size; i++) {
            data[i].~T();
        }
        _size = 0;
    }

    T * begin() {
        return data;
    }

    const T * begin() const {
        return data;
    }

    T * end() {
        return data + _size;
    }

    const T * end() const {
        return data + _size;
    }

    static void Free(T* ptr, int num) {
        if (ptr == nullptr) return;
        for (int i = 0; i < num; i++) {
            ptr[i].~T();
        }
        free(ptr);
    }

    ~Vector() {
        Free(data, _size);
    }
};
