#include <iostream>
#include <stdexcept>

class Date {
public:
    int days;

    Date(int _day, int _month, int _year) {
        if (InvalidDate(_day, _month, _year)) {
            throw std::invalid_argument("Invalid");
        }
        days = CalcOfDays(_day, _month, _year);
    }

    int GetDay() const {
        int _days = 0;
        size_t _year = 1970;
        for (; _days + 365 + IsLeap(_year) < days; _year++) {
            _days += 365 + IsLeap(_year);
        }
        size_t _month = 1;
        for (; _days + DaysInMonth(_month, _year) < days; _month++) {
            _days += DaysInMonth(_month, _year);
        }
        return days - _days;
    }

    int GetMonth() const {
        int _days = 0;
        size_t _year = 1970;
        for (; _days + 365 + IsLeap(_year) < days; _year++) {
            _days += 365 + IsLeap(_year);
        }
        size_t _month = 1;
        for (; _days + DaysInMonth(_month, _year) < days; _month++) {
            _days += DaysInMonth(_month, _year);
        }
        return _month;
    }

    int GetYear() const {
        int _days = 0;
        size_t _year = 1970;
        for (; _days + 365 + IsLeap(_year) < days; _year++) {
            _days += 365 + IsLeap(_year);
        }
        return _year;
    }

    void Print() {
        std::cout << GetDay() << " " << GetMonth() << " " << GetYear() << '\n';
    }

    friend Date& operator++(Date& x);

    friend Date& operator--(Date& x);

    friend Date operator+(Date& left, int right);

    friend Date operator+(int left, Date& right);

    friend Date operator-(Date& left, int right);

    friend Date operator-(int left, Date& right);

    friend int operator-(Date& left, Date& right);

private:
    static int DaysInMonth(int month, int year) {
        bool add = ((IsLeap(year)) && (month == 2));
        return 28 + (month + month/8)%2 + 2 % month + 2*(1 / month) + add;
    }

    static bool IsLeap(int year) {
        return (year%400 == 0 || (year%100 != 0 && year%4 == 0));
    }

    static bool InvalidDate(int _day, int _month, int _year) {
        return !((_year >= 1970 && _year <= 2099) &&
                 (_month >= 1 && _month <= 12) &&
                 (_day >= 1 && _day <= DaysInMonth(_month, _year)));
    }

    static int CalcOfDays(int day, int month, int year) {
        int days = 0;
        for (size_t _month = 1; _month < month; _month++) {
            days += DaysInMonth(_month, year);
        }
        for (size_t _year = 1970; _year < year; _year++) {
            days += 365 + IsLeap(_year);
        }
        days += day;
        return days;
    }
};

Date& operator++(Date& x) {
    x.days++;
    return x;
}

Date& operator--(Date& x) {
    x.days--;
    return x;
}

Date operator+(Date& left, int right) {
    Date result = left;
    result.days += right;
    return result;
}

Date operator+(int left, Date& right) {
    Date result = right;
    result.days += left;
    return result;
}

Date operator-(Date& left, int right) {
    Date result = left;
    result.days -= right;
    return result;
}

Date operator-(int left, Date& right) {
    Date result = right;
    result.days -= left;
    return result;
}

int operator-(Date& left, Date& right) {
    return left.days - right.days;
}
