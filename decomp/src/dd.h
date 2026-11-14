#pragma once
#include <cmath>
#include <iostream>

struct DD {
    double hi, lo;

    DD() : hi(0.0), lo(0.0) {}
    DD(double h, double l) : hi(h), lo(l) {}
    DD(double h) : hi(h), lo(0.0) {}

    explicit operator double() const { return hi; }
};

inline DD two_sum(double a, double b) {
    double hi = a + b;
    double a_main = hi - b;
    double b_main = hi - a_main;
    double lo = (a - a_main) + (b - b_main);
    return DD(hi, lo);
}

inline DD two_prod(double a, double b) {
    double hi = a * b;
    double lo = std::fma(a, b, -hi);
    return DD(hi, lo);
}

inline DD operator+(const DD& x, const DD& y) {
    DD s = two_sum(x.hi, y.hi);
    DD t = two_sum(x.lo, y.lo);
    double c = s.lo + t.hi;
    DD v = two_sum(s.hi, c);
    double w = t.lo + v.lo;
    return two_sum(v.hi, w);
}

inline DD operator-(const DD& x, const DD& y) {
    DD s = two_sum(x.hi, -y.hi);
    DD t = two_sum(x.lo, -y.lo);
    double c = s.lo + t.hi;
    DD v = two_sum(s.hi, c);
    double w = t.lo + v.lo;
    return two_sum(v.hi, w);
}

inline DD operator*(const DD& x, const DD& y) {
    DD p = two_prod(x.hi, y.hi);
    double e = x.hi * y.lo + x.lo * y.hi + p.lo;
    return two_sum(p.hi, e);
}

inline DD operator/(const DD& x, const DD& y) {
    double q1 = x.hi / y.hi;
    DD r = x - y * q1;
    double q2 = r.hi / y.hi;
    r = r - y * q2;
    double q3 = r.hi / y.hi;
    return two_sum(q1, q2) + q3;
}

inline DD operator+(const DD& x, double y) { return x + DD(y); }
inline DD operator+(double x, const DD& y) { return DD(x) + y; }
inline DD operator-(const DD& x, double y) { return x - DD(y); }
inline DD operator-(double x, const DD& y) { return DD(x) - y; }
inline DD operator*(const DD& x, double y) { return x * DD(y); }
inline DD operator*(double x, const DD& y) { return DD(x) * y; }
inline DD operator/(const DD& x, double y) { return x / DD(y); }
inline DD operator/(double x, const DD& y) { return DD(x) / y; }

inline DD& operator+=(DD& x, const DD& y) { return x = x + y; }
inline DD& operator-=(DD& x, const DD& y) { return x = x - y; }
inline DD& operator*=(DD& x, const DD& y) { return x = x * y; }
inline DD& operator/=(DD& x, const DD& y) { return x = x / y; }
inline DD operator-(const DD& x) { return DD(-x.hi, -x.lo); }

inline bool operator>(const DD& x, const DD& y) {
    if (x.hi > y.hi) return true;
    if (x.hi < y.hi) return false;
    return x.lo > y.lo;
}
inline bool operator<(const DD& x, const DD& y) { return y > x; }
inline bool operator==(const DD& x, const DD& y) { return x.hi == y.hi && x.lo == y.lo; }
inline bool operator!=(const DD& x, const DD& y) { return !(x == y); }
inline bool operator<=(const DD& x, const DD& y) { return !(x > y); }
inline bool operator>=(const DD& x, const DD& y) { return !(x < y); }

inline DD abs(const DD& x) {
    return x.hi >= 0 ? x : DD(-x.hi, -x.lo);
}

inline DD sqrt(const DD& x) {
    if (x.hi == 0.0 && x.lo == 0.0) return DD(0.0);
    double s1 = std::sqrt(x.hi);
    DD s = two_prod(s1, s1);
    DD r = x - s;
    double s2 = r.hi / (2.0 * s1);
    return two_sum(s1, s2);
}

inline std::ostream& operator<<(std::ostream& os, const DD& x) {
    os << x.hi;
    return os;
}