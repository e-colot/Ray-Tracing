#pragma once

class Vector {
    // Attributes
private:
    float x; // X-component of the vector
    float y; // Y-component of the vector

    // Constructors
public:
    Vector(); // Default constructor
    Vector(int x, int y); // Constructor with integer coordinates
    Vector(float x, float y); // Constructor with floating-point coordinates

    // Operators
public:
    Vector operator+(const Vector& other) const; // Addition operator
    Vector operator-(const Vector& other) const; // Subtraction operator
    Vector operator/(float scalar) const; // Division by float scalar
    Vector operator/(int scalar) const; // Division by integer scalar
    bool operator==(const Vector& other) const; // Equality comparison operator

    // Accessors
public:
    float get_x() const; // Returns the x-component of the vector
    float get_y() const; // Returns the y-component of the vector
    float get_norm() const; // Returns the norm (magnitude) of the vector

    // Methods
public:
    void normalize(); // Normalizes the vector to have a magnitude of 1
    float scalar_prod(const Vector& other) const; // Calculates the scalar product (dot product) with another vector
    Vector perpendicular() const; // Returns a vector perpendicular to the current vector
    void show(bool finish_new_line = true) const; // Prints the vector components
    float squared_norm() const; // Returns the square of the vector's norm

private:
    float calc_norm() const; // Calculates the norm of the vector (private helper method)

};

// Commutative operators
Vector operator*(int scalar, const Vector& vect); // Scalar multiplication (commutative)
Vector operator*(const Vector& vect, int scalar); // Scalar multiplication (commutative)
Vector operator*(float scalar, const Vector& vect); // Scalar multiplication (commutative)
Vector operator*(const Vector& vect, float scalar); // Scalar multiplication (commutative)
