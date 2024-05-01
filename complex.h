#pragma once

#include "constants.h"

class Complex;

class Complex {

    // ---------- CONSTRUCTORS ----------

public:
    Complex();
    Complex(float real);
    Complex(int real);
    Complex(float real, float imaginary);
    Complex(double real, double imaginary);
    Complex(int real, int imaginary);

    // ---------- OPERATORS ----------

public:
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;
    void operator*=(const Complex& other);
    void operator+=(const Complex& other);

    // ---------- ACCESSORS ----------

public:
    double get_real() const;
    double get_imag() const;

    // ---------- METHODS ----------

public:
    Complex cplx_sqrt() const;
    void show() const;
    double squared_norm() const;
private:
    Complex conjugate() const;

    // ---------- ATTRIBUTES ----------

private:
    double re;
    double im;

};

// ---------- COMMUTATIVE OPERATORS ----------

Complex operator*(int scalar, const Complex& complex);
Complex operator*(const Complex& complex, int scalar);
Complex operator*(float scalar, const Complex& complex);
Complex operator*(const Complex& complex, float scalar);
Complex operator*(double scalar, const Complex& complex);
Complex operator*(const Complex& complex, double scalar);
Complex operator/(int scalar, const Complex& complex);
Complex operator/(const Complex& complex, int scalar);
Complex operator/(float scalar, const Complex& complex);
Complex operator/(const Complex& complex, float scalar);
Complex operator/(double scalar, const Complex& complex);
Complex operator/(const Complex& complex, double scalar);
Complex operator+(int scalar, const Complex& complex);
Complex operator+(const Complex& complex, int scalar);
Complex operator+(float scalar, const Complex& complex);
Complex operator+(const Complex& complex, float scalar);
Complex operator+(double scalar, const Complex& complex);
Complex operator+(const Complex& complex, double scalar);
Complex operator-(int scalar, const Complex& complex);
Complex operator-(const Complex& complex, int scalar);
Complex operator-(float scalar, const Complex& complex);
Complex operator-(const Complex& complex, float scalar);
Complex operator-(double scalar, const Complex& complex);
Complex operator-(const Complex& complex, double scalar);

// ---------- OTHER ----------

Complex cplx_exp(const Complex& complex);
