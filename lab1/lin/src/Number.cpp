#include "Number.h"

Number::Number(double d) : val(d) {}
Number::Number(const Number& rhs) : val(rhs.val) {}

double Number::getVal() const {
    return val;
}

void Number::assign(const Number& rhs) {
    val = rhs.val;
}

Number Number::add(const Number& rhs) const {
    return Number(val + rhs.val);
}
Number Number::sub(const Number& rhs) const {
    return Number(val - rhs.val);
}
Number Number::mul(const Number& rhs) const {
    return Number(val * rhs.val);
}
Number Number::div(const Number& rhs) const {
    return Number(val / rhs.val);
}

const Number& Number::Zero() {
    static const Number zero(0.0);
    return zero;
}

const Number& Number::One() {
    static const Number one(1.0);
    return one;
}