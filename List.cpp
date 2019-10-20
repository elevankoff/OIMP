#include <iostream>

template <typename T>
class SharedPtr {
    struct node {
        int count;
        node () : count(0) {}
        node (int n) : count(n) {}
    };

public:
    T * ptr;
    node * counter;

    SharedPtr() : ptr(nullptr), counter(new node(1)) {}

    SharedPtr(T * _ptr) : ptr(_ptr), counter(new node(1)) {}

    SharedPtr(const SharedPtr& other) {
        ptr = other.ptr;
        counter = other.counter;
        counter->count++;
    }

    SharedPtr(SharedPtr&& other) {
        ptr = std::move(other.ptr);
        counter = std::move(other.counter);
        other.ptr = nullptr;
        other.counter = nullptr;
    }

    SharedPtr& operator=(T * other) {
        if (other == ptr) {
            counter->count++;
        } else
        if (ptr == nullptr) {
            ptr(other);
        } else {
            counter->count--;
            if (counter->count == 0) {
                delete ptr;
                delete counter;
            }
            ptr = other;
            counter = new node(1);
        }
        return *this;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (other.ptr == ptr) {
            return *this;
        } else
        if (ptr == nullptr) {
            delete counter;
            ptr = other.ptr;
            counter = other.counter;
            counter->count++;
        } else {
            counter->count--;
            if (counter->count == 0) {
                delete ptr;
                delete counter;
            }
            ptr = other.ptr;
            counter = other.counter;
            counter->count++;
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (other.ptr == ptr) {
            return *this;
        } else
        if (ptr == nullptr) {
            ptr = other.ptr;
            delete counter;
            counter = other.counter;
            other.ptr = nullptr;
            other.counter = nullptr;
        } else {
            counter->count--;
            if (counter->count == 0) {
                delete ptr;
                delete counter;
            }
            ptr = other.ptr;
            counter = other.counter;
            other.ptr = nullptr;
            other.counter = nullptr;
        }
        return *this;
    }

    T& operator*() {
        return *ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T * operator->() const {
        return ptr;
    }

    void reset(T * _ptr) {
        counter->count--;
        if (counter->count == 0) {
            delete ptr;
            delete counter;
        }
        counter = new node(1);
        ptr = _ptr;
    }

    void swap(SharedPtr& other) {
        std::swap(ptr, other.ptr);
        std::swap(counter, other.counter);
    }

    T * get() const {
        return ptr;
    }

    explicit operator bool() const {
        return (ptr != nullptr);
    }

    ~SharedPtr() {
        if (counter == nullptr) return;
        counter->count--;
        if (counter->count == 0) {
            delete ptr;
            delete counter;
        }
    }
};
