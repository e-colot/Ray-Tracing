#pragma once

class Vector;

class Vector {

    // ---------- CONSTRUCTORS ----------

public:
    Vector();
    Vector(int x, int y);
    Vector(float x, float y);

    // ---------- OPERATORS ----------

public:
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator/(float scalar) const;
    Vector operator/(int scalar) const;

    // ---------- ACCESSORS ----------

public:
    float get_x() const;
    float get_y() const;
    float get_norm() const;

    // ---------- MUTATORS ----------

public:
    void set(float x_value, float y_value);
    void set(Vector other);

    // ---------- METHODS ----------

public:
    void normalize();
    float scalar_prod(const Vector& other) const;
    Vector perpendicular() const;
    void show(bool finish_new_line = true) const;
    float squared_norm() const;
private:
    float calc_norm() const;

    // ---------- ATTRIBUTES ----------

private:
    float x;
    float y;

};

// ---------- COMMUTATIVE OPERATORS ----------

Vector operator*(int scalar, const Vector& vect);
Vector operator*(const Vector& vect, int scalar);
Vector operator*(float scalar, const Vector& vect);
Vector operator*(const Vector& vect, float scalar);
