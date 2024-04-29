#include "complex.h"
#include "math.h"
#include <iostream>

#define SIGN(a) ((a>=0)? 1 : -1)

// ---------- CONSTRUCTORS ----------

Complex::Complex() : re(0.0f), im(0.0f) {}
Complex::Complex(float r) : re(r), im(0.0f) {}
Complex::Complex(int r) : re(static_cast<float>(r)), im(0.0f) {}
Complex::Complex(float r, float i) : re(r), im(i) {}
Complex::Complex(int r, int i) : re(static_cast<float>(r)), im(static_cast<float>(i)) {}

// ---------- OPERATORS ----------

Complex Complex::operator+(const Complex& c) const {
    return Complex(re + c.get_real(), im + c.get_imag());
}
Complex Complex::operator-(const Complex& c) const {
    return Complex(re - c.get_real(), im - c.get_imag());
}
Complex Complex::operator*(const Complex& c) const {
    return Complex((re * c.get_real()) - (im * c.get_imag()), (re * c.get_imag()) + (im * c.get_real()));
}
Complex Complex::operator/(const Complex& c) const {
    return (*this * c.conjugate()) / c.squared_norm();
}
void Complex::operator*=(const Complex& c) {
    float old_re = re;
    re = (re * c.get_real()) - (im * c.get_imag());
    im = (old_re * c.get_imag()) + (im * c.get_real());
}
void Complex::operator+=(const Complex& c) {
    re = re + c.get_real();
    im = im + c.get_imag();
}

// ---------- ACCESSORS ----------

float Complex::get_real() const {
    return re;
}
float Complex::get_imag() const {
    return im;
}

// ---------- METHODS ----------

Complex Complex::cplx_sqrt() const {
    if (im <= std::numeric_limits<float>::epsilon()) {
        // checks if im == 0
        if (re >= 0) {
            return Complex(sqrt(re), 0.0f);
        }
        else {
            return Complex(0.0f, sqrt(-re));
        }
    }
    else {
        return Complex(sqrt((sqrt(squared_norm()) + re) / 2), (SIGN(im)) * sqrt((sqrt(squared_norm()) - re) / 2));
    }
}
void Complex::show() const {
    if ((SIGN(im)) == 1) {
        std::cout << re << " + j" << im << std::endl;
    }
    else {
        std::cout << re << " - j" << -im << std::endl;
    }
}
float Complex::squared_norm() const {
    return (re * re + im * im);
}
Complex Complex::conjugate() const {
    return Complex(re, -im);
}

// ---------- COMMUTATIVE OPERATORS ----------

Complex operator*(int a, const Complex& v) {
    return Complex(a, 0) * v;
}
Complex operator*(const Complex& v, int a) {
    return Complex(a, 0) * v;
}
Complex operator*(float a, const Complex& v) {
    return Complex(a, 0.0f) * v;
}
Complex operator*(const Complex& v, float a) {
    return Complex(a, 0.0f) * v;
}
Complex operator/(int a, const Complex& v) {
    return Complex(a, 0) / v;
}
Complex operator/(const Complex& v, int a) {
    return Complex(v.get_real() / a, v.get_imag() / a);
}
Complex operator/(float a, const Complex& v) {
    return Complex(a, 0.0f) / v;
};
Complex operator/(const Complex& v, float a) {
    return Complex(v.get_real() / a, v.get_imag() / a);
}
Complex operator+(int a, const Complex& v) {
    return Complex(a, 0) + v;
}
Complex operator+(const Complex& v, int a) {
    return Complex(a, 0) + v;
}
Complex operator+(float a, const Complex& v) {
    return Complex(a, 0.0f) + v;
}
Complex operator+(const Complex& v, float a) {
    return Complex(a, 0.0f) + v;
}
Complex operator-(int a, const Complex& v) {
    return Complex(a, 0) - v;
}
Complex operator-(const Complex& v, int a) {
    return v - Complex(a, 0);
}
Complex operator-(float a, const Complex& v) {
    return Complex(a, 0.0f) - v;
}
Complex operator-(const Complex& v, float a) {
    return  v - Complex(a, 0.0f);
}

// ---------- OTHER ----------

Complex cplx_exp(const Complex& c) {
    return (exp(c.get_real()) * Complex(cos(c.get_imag()), sin(c.get_imag())));
}
