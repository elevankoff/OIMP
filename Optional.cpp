#include <iostream>
#include <string>

// Это исключение нужно генерировать в функции value
// при обращении к пустому объекту Optional
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;

    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T&& elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        new (data) (unsigned char)(*other.data);
        defined = true;
    }

    Optional& operator=(const Optional& other) {
        if (defined) {
            if (other.defined) {
                T * ptr1 = (T*) (other.data);
                T * ptr2 = (T*) data;
                *ptr1 = *ptr2;
            } else {
                reset();
            }
        } else {
            if (other.defined) {
                T * ptr1 = (T*) (other.data);
                new (data) T(*ptr1);
                defined = true;
            }
        }
        return *this;
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            T * ptr1 = (T*) (data);
            *ptr1 = elem;
        } else {
            new (data) T(elem);
        }
        defined = true;
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            T * ptr1 = (T*) (data);
            *ptr1 = std::move(elem);
        } else {
            new (data) T(std::move(elem));
        }
        defined = true;
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        T * ptr = (T*) (data + 0);
        return *ptr;
    }

    const T& operator*() const {
        T * ptr = (T*) (data + 0);
        return *ptr;
    }

    T* operator->() {
        T * ptr = (T*) (data + 0);
        return ptr;
    }

    const T* operator->() const {
        T * ptr = (T*) (data + 0);
        return ptr;
    }

    T& value() {
        if (!defined) throw BadOptionalAccess();
        T * ptr = (T*) (data + 0);
        return *ptr;
    }

    const T& value() const {
        if (!defined) throw BadOptionalAccess();
        T * ptr = (T*) (data + 0);
        return *ptr;
    }

    void reset() {
        if (defined) {
            reinterpret_cast<T *>(data)->~T();
            defined = false;
        }
    }

    ~Optional() {
        if (defined) {
            reinterpret_cast<T *>(data)->~T();
        }
    }
};

