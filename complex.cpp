#include "complex.h"
#include "math.h"
#include <iostream>

#define SIGN(a) ((a>=0)? 1 : -1)

// Constructors

Complex::Complex() : re(0.0f), im(0.0f) {}
Complex::Complex(float r) : re(r), im(0.0f) {}
Complex::Complex(int r) : re(static_cast<double>(r)), im(0.0) {}
Complex::Complex(float r, float i) : re(static_cast<double>(r)), im(static_cast<double>(i)) {}
Complex::Complex(double r, double i) : re(r), im(i) {}
Complex::Complex(int r, int i) : re(static_cast<double>(r)), im(static_cast<double>(i)) {}

// Operators

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
    double old_re = re;
    re = (re * c.get_real()) - (im * c.get_imag());
    im = (old_re * c.get_imag()) + (im * c.get_real());
}
void Complex::operator+=(const Complex& c) {
    re = re + c.get_real();
    im = im + c.get_imag();
}
bool Complex::operator==(const Complex& other) const
{
    return ((*this - other).squared_norm() <= 1e-12f);
    // differnce smaller than 1e-6
}

// Accessors

double Complex::get_real() const {
    return re;
}
double Complex::get_imag() const {
    return im;
}

// Methods

Complex Complex::cplx_sqrt() const {
    if (im <= std::numeric_limits<float>::epsilon()) {
        // checks if im == 0
        if (re >= 0) {
            return Complex(sqrt(re), 0.0);
        }
        else {
            return Complex(0.0, sqrt(-re));
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
double Complex::squared_norm() const {
    return (re * re + im * im);
}
Complex Complex::conjugate() const {
    return Complex(re, -im);
}

// Commutative operators

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
Complex operator*(double a, const Complex& v) {
    return Complex(a, 0.0) * v;
}
Complex operator*(const Complex& v, double a) {
    return Complex(a, 0.0) * v;
}
Complex operator/(int a, const Complex& v) {
    return Complex(a, 0) / v;
}
Complex operator/(const Complex& v, int a) {
    return Complex(v.get_real() / a, v.get_imag() / a);
}
Complex operator/(float a, const Complex& v) {
    return Complex(a, 0.0f) / v;
}
Complex operator/(const Complex& v, float a) {
    return Complex(v.get_real() / a, v.get_imag() / a);
}
Complex operator/(double a, const Complex& v) {
    return Complex(a, 0.0) / v;
}
Complex operator/(const Complex& v, double a) {
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
Complex operator+(double a, const Complex& v) {
    return Complex(a, 0.0) + v;
}
Complex operator+(const Complex& v, double a) {
    return Complex(a, 0.0) + v;
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
Complex operator-(double a, const Complex& v) {
    return Complex(a, 0.0) - v;
}
Complex operator-(const Complex& v, double a) {
    return  v - Complex(a, 0.0);
}

// Other

Complex cplx_exp(const Complex& c) {
    return (exp(c.get_real()) * Complex(cos(c.get_imag()), sin(c.get_imag())));
}
