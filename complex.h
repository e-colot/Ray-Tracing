#pragma once

#include "constants.h"

class Complex {
    // Attributes
private:
    double re; // Real part of the complex number
    double im; // Imaginary part of the complex number

    // Constructors
public:
    Complex(); // Default constructor
    Complex(float real); // Constructor with real part as float
    Complex(int real); // Constructor with real part as int
    Complex(float real, float imaginary); // Constructor with real and imaginary parts as floats
    Complex(double real, double imaginary); // Constructor with real and imaginary parts as doubles
    Complex(int real, int imaginary); // Constructor with real and imaginary parts as ints

    // Operators
public:
    Complex operator+(const Complex& other) const; // Addition operator
    Complex operator-(const Complex& other) const; // Subtraction operator
    Complex operator*(const Complex& other) const; // Multiplication operator
    Complex operator/(const Complex& other) const; // Division operator
    void operator*=(const Complex& other); // Multiplication assignment operator
    void operator+=(const Complex& other); // Addition assignment operator

    // Accessors
public:
    double get_real() const; // Returns the real part of the complex number
    double get_imag() const; // Returns the imaginary part of the complex number

    // Methods
public:
    Complex cplx_sqrt() const; // Returns the square root of the complex number
    void show() const; // Displays the complex number
    double squared_norm() const; // Returns the square of the norm (magnitude) of the complex number

private:
    Complex conjugate() const; // Returns the conjugate of the complex number
};

// Commutative operators
Complex operator*(int scalar, const Complex& complex); // Scalar multiplication (commutative)
Complex operator*(const Complex& complex, int scalar); // Scalar multiplication (commutative)
Complex operator*(float scalar, const Complex& complex); // Scalar multiplication (commutative)
Complex operator*(const Complex& complex, float scalar); // Scalar multiplication (commutative)
Complex operator*(double scalar, const Complex& complex); // Scalar multiplication (commutative)
Complex operator*(const Complex& complex, double scalar); // Scalar multiplication (commutative)
Complex operator/(int scalar, const Complex& complex); // Scalar division (commutative)
Complex operator/(const Complex& complex, int scalar); // Scalar division (commutative)
Complex operator/(float scalar, const Complex& complex); // Scalar division (commutative)
Complex operator/(const Complex& complex, float scalar); // Scalar division (commutative)
Complex operator/(double scalar, const Complex& complex); // Scalar division (commutative)
Complex operator/(const Complex& complex, double scalar); // Scalar division (commutative)
Complex operator+(int scalar, const Complex& complex); // Scalar addition (commutative)
Complex operator+(const Complex& complex, int scalar); // Scalar addition (commutative)
Complex operator+(float scalar, const Complex& complex); // Scalar addition (commutative)
Complex operator+(const Complex& complex, float scalar); // Scalar addition (commutative)
Complex operator+(double scalar, const Complex& complex); // Scalar addition (commutative)
Complex operator+(const Complex& complex, double scalar); // Scalar addition (commutative)
Complex operator-(int scalar, const Complex& complex); // Scalar subtraction (commutative)
Complex operator-(const Complex& complex, int scalar); // Scalar subtraction (commutative)
Complex operator-(float scalar, const Complex& complex); // Scalar subtraction (commutative)
Complex operator-(const Complex& complex, float scalar); // Scalar subtraction (commutative)
Complex operator-(double scalar, const Complex& complex); // Scalar subtraction (commutative)
Complex operator-(const Complex& complex, double scalar); // Scalar subtraction (commutative)

// Other
Complex cplx_exp(const Complex& complex); // Exponential of a complex number
