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
    float get_real() const;
    float get_imag() const;

    // ---------- METHODS ----------

public:
    Complex cplx_sqrt() const;
    void show() const;
    float squared_norm() const;
private:
    Complex conjugate() const;

    // ---------- ATTRIBUTES ----------

private:
    float re;
    float im;

};

// ---------- COMMUTATIVE OPERATORS ----------

Complex operator*(int scalar, const Complex& complex);
Complex operator*(const Complex& complex, int scalar);
Complex operator*(float scalar, const Complex& complex);
Complex operator*(const Complex& complex, float scalar);
Complex operator/(int scalar, const Complex& complex);
Complex operator/(const Complex& complex, int scalar);
Complex operator/(float scalar, const Complex& complex);
Complex operator/(const Complex& complex, float scalar);
Complex operator+(int scalar, const Complex& complex);
Complex operator+(const Complex& complex, int scalar);
Complex operator+(float scalar, const Complex& complex);
Complex operator+(const Complex& complex, float scalar);
Complex operator-(int scalar, const Complex& complex);
Complex operator-(const Complex& complex, int scalar);
Complex operator-(float scalar, const Complex& complex);
Complex operator-(const Complex& complex, float scalar);

// ---------- OTHER ----------

Complex cplx_exp(const Complex& complex);
