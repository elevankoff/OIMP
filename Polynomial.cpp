#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>

namespace bool_shit {
    bool need_sign(int pos) {
        return (pos != 0);
    }

    template <typename T>
    bool need_T(int power, const T& x) {
        return !(x == T(1) || x == -T(1)) || (power == 0);
    }

    bool need_mult(bool need_T, int power) {
        if (power == 0) return false;
        return need_T;
    }
    bool need_x(bool need_mult, int power) {
        if (need_mult) return true;

        return (power != 0);
    }
    bool need_power(int power) {
        return (power > 1);
    }

    template <typename T>
    std::string sign(const T& x) {
        return (x > T(0)) ? "+" : "-";
    }
}
using namespace bool_shit;

template <typename T>
class Polynomial {
public:
    mutable std::map<int, T> coefficients;

    explicit Polynomial(std::vector<T> _coefficients) {
        for (size_t i = 0; i < _coefficients.size(); i++) {
            if (_coefficients[i] != T(0)) {
                coefficients[i] = _coefficients[i];
            }
        }
    }
    explicit Polynomial(T x) {
        if (x != T(0)) coefficients[0] = x;
    }
    explicit Polynomial() = default;
    Polynomial(int power, T val) {
        if (val != T(0)) coefficients[power] = val;
    }
    template<typename Iter>
    Polynomial(Iter first, Iter last) {
        int power = 0;
        std::for_each(first, last, [&] (const T& now){
            if (now != T(0)) coefficients[power] = now;
            power++;
        });
    }

    int Degree() const {
        if (coefficients.empty()) return -1;
        return (*coefficients.rbegin()).first;
    }

    typename std::map<int, T>::const_iterator begin() const {
        return coefficients.cbegin();
    }
    typename std::map<int, T>::const_iterator end() const {
        return coefficients.cend();
    }

    T operator[](const int& i) const {
        if (!coefficients.count(i)) return T(0);
        return coefficients[i];
    }
    T operator()(const T& x) const {
        T result = T(0), power = T(1);
        int power_s = 0;
        std::for_each(coefficients.begin(), coefficients.end(),
                      [&result, &power, &x, &power_s](const std::pair<int, T>& now){
            while (power_s < now.first) {
                power_s++;
                power *= x;
            }
            result += now.second * power;
        });
        return result;
    }

    template <typename T1>
    Polynomial<T>& operator+=(const T1& _other) {
        Polynomial<T> other(_other);
        for (auto it = other.begin(); it != other.end(); it++) {
            coefficients[(*it).first] += (*it).second;
            if (coefficients[(*it).first] == T(0)) {
                coefficients.erase((*it).first);
            }
        }
        return *this;
    }

    template <typename T1>
    Polynomial<T>& operator-=(const T1& _other) {
        Polynomial<T> other(_other);
        for (auto it = other.begin(); it != other.end(); it++) {
            coefficients[(*it).first] -= (*it).second;
            if (coefficients[(*it).first] == T(0)) {
                coefficients.erase((*it).first);
            }
        }
        return *this;
    }

    template <typename T1>
    Polynomial<T>& operator*=(const T1& _other) {
        Polynomial<T> other(_other), result;
        for (auto it1 = coefficients.begin(); it1 != coefficients.end(); it1++) {
            for (auto it2 = other.begin(); it2 != other.end(); it2++) {
                int pos = (*it1).first + (*it2).first;
                result.coefficients[pos] += (*it1).second * (*it2).second;
                if (result.coefficients[pos] == T(0)) {
                    result.coefficients.erase(pos);
                }
            }
        }
        *this = result;
        return *this;
    }

    template <typename T1, typename T2>
    friend Polynomial<T> operator+(const T1& first, const T2& second) {
        Polynomial<T> result(first);
        result += Polynomial<T>(second);
        return result;
    }

    template <typename T1, typename T2>
    friend Polynomial<T> operator-(const T1& first, const T2& second) {
        Polynomial<T> result(first);
        result -= Polynomial<T>(second);
        return result;
    }

    template <typename T1, typename T2>
    friend Polynomial<T> operator*(const T1& first, const T2& second) {
        Polynomial<T> result = Polynomial<T>(first);
        result *= Polynomial<T>(second);
        return result;
    }

    friend bool operator==(const Polynomial<T>& first, const Polynomial<T>& second) {
        return first.coefficients == second.coefficients;
    }
    friend bool operator==(const Polynomial<T>& first, const T& second) {
        return (first == Polynomial<T>(second));
    }
    friend bool operator==(const T& first, const Polynomial<T>& second) {
        return (Polynomial<T>(second) == Polynomial<T>(first));
    }

    friend bool operator!=(const Polynomial<T>& first, const Polynomial<T>& second) {
        return !(first == second);
    }
    friend bool operator!=(const Polynomial<T>& first, const T& second) {
        return !(first == second);
    }
    friend bool operator!=(const T& first, const Polynomial<T>& second) {
        return !(first == second);
    }

    friend Polynomial<T>operator&(const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> result, SecondInPower(T(1));
        int power_s = 0;
        for (auto it = first.begin(); it != first.end(); it++) {
            while (power_s < (*it).first) {
                power_s++;
                SecondInPower *= second;
            }
            result += (*it).second * SecondInPower;
        }
        return result;
    }

    friend Polynomial<T>operator/(const Polynomial<T>& first, const Polynomial<T>& second) {
        if (first.Degree() >= second.Degree()) {
            return  Polynomial<T>(first.Degree() - second.Degree(),
                    (*first.coefficients.rbegin()).second / (*second.coefficients.rbegin()).second)
                    + (first - (second * Polynomial<T>(
                            first.Degree() - second.Degree(),
                    (*first.coefficients.rbegin()).second /
                    (*second.coefficients.rbegin()).second)))
                    / second;
        }
        return Polynomial<T>();
    }

    friend Polynomial<T>operator%(const Polynomial<T>& first, const Polynomial<T>& second) {
        return first - (first / second) * second;
    }

    friend Polynomial<T>operator,(const Polynomial<T>& first, const Polynomial<T>& second) {
        return (second == Polynomial<T>()) ? (
                first/Polynomial((*first.coefficients.rbegin()).second)) : (second, first%second);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, Polynomial<T> other) {
    std::vector<std::pair<int, T>> result;
    for (auto j : other.coefficients) {
        if (j.second != T(0)) {
            result.push_back(j);
        }
    }
    if (result.empty()) {
        out << 0;
        return out;
    }
    std::reverse(result.begin(), result.end());
    for (size_t i = 0; i < result.size(); i++) {
        T now = result[i].second;
        int power = result[i].first;
        if (need_sign(i)) {
            out << sign(now);
        } else {
            if (now < T(0)) out << '-';
        }
        if (now < T(0)) now *= -T(1);
        if (need_T(power, now)) out << now;
        if (need_mult(need_T(power, now), power)) out << '*';
        if (need_x(need_mult(need_T(power, now), power), power)) out << "x";
        if (need_power(power)) out << "^" << power;
    }
    return out;
}
