#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>

template <typename T>
class Matrix {
public:
    std::vector<T> elements;
    size_t N, M;

    explicit Matrix(const std::vector<std::vector<T>>& _elements) {
        N = _elements.size();
        M = _elements[0].size();
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                elements.push_back(_elements[i][j]);
            }
        }
    }

    typename std::vector<T>::iterator begin() {
        return elements.begin();
    }

    typename std::vector<T>::iterator end() {
        return elements.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return elements.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return elements.end();
    }

    Matrix(const int _N, const int _M) {
        N = _N;
        M = _M;
        elements.resize(N * M, T(0));
    }

    T& operator()(const int i, const int j) {
        return elements[i * M + j];
    }
    const T& operator()(const int i, const int j) const {
        return elements[i * M + j];
    }

    std::pair<size_t, size_t> size() const {
        return {N, M};
    }

    Matrix& operator+=(const Matrix& other) {
        size_t numberOfLines = (*this).size().first, elementsInLines = (*this).size().second;
        for (size_t i = 0; i < numberOfLines; i++) {
            for (size_t j = 0; j < elementsInLines; j++) {
                *this(i, j) += other(i, j);
            }
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& other) {
        size_t thisN = (*this).size().first, thisM = (*this).size().second;
        size_t otherN = other.size().first, otherM = other.size().second;
        assert(thisM == otherN);
        Matrix result(thisN, otherM);
        for (size_t i = 0; i < thisN; i++) {
            for (size_t j = 0; j < otherM; j++) {
                for (size_t dx = 0; dx < thisM; dx++) {
                    result(i, j) += (*this)(i, dx) * other(dx, j);
                }
            }
        }
        *this = result;
        return *this;
    }

    Matrix operator*(const Matrix& other) {
        Matrix result = *this;
        result *= other;
        return result;
    }

    Matrix operator+(const Matrix& second) const {
        Matrix result = *this;
        result += second;
        return result;
    }

    Matrix& transpose() {
        size_t numberOfLines = (*this).size().first, elementsInLine = (*this).size().second;
        Matrix result(elementsInLine, numberOfLines);
        for (size_t i = 0; i < numberOfLines; i++) {
            for (size_t j = 0; j < elementsInLine; j++) {
                result(j, i) = *this(i, j);
            }
        }
        *this = result;
        return *this;
    }

    Matrix transposed() const {
        Matrix result = (*this);
        return result.transpose();
    }

    template <typename T1>
    Matrix& operator*=(const T1& scalar) {
        size_t numberOfLines = (*this).size().first, elementsInLines = (*this).size().second;
        for (size_t i = 0; i < numberOfLines; i++) {
            for (size_t j = 0; j < elementsInLines; j++) {
                *this(i, j) *= scalar;
            }
        }
        return *this;
    }

    template <typename T1>
    friend Matrix operator*(const Matrix& matrix, const T1& scalar) {
        Matrix result = matrix;
        result *= scalar;
        return result;
    }

    template <typename T1>
    friend Matrix operator*(const T1& scalar, const Matrix& matrix) {
        return matrix * scalar;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix) {
    size_t numberOfLines = matrix.size().first, elementsInLine = matrix.size().second;
    for (size_t i = 0; i < numberOfLines; i++) {
        for (size_t j = 0; j < elementsInLine; j++) {
            if (!j) {
                out << matrix(i, j);
            } else {
                out << '\t' << matrix(i, j);
            }
        }
        if (i + 1 != numberOfLines) out << '\n';
    }
    return out;
}
