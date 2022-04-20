#pragma once
#include <iostream>
#include <string>

using std::string, std::ostream;

class Vector2D {

public:
    double x, y;

    Vector2D(double x, double y);
    Vector2D(const Vector2D& v);
    Vector2D();
    ~Vector2D();

    double length();
    double manhattanLength();
    Vector2D normalize();
    Vector2D truncate(double max);
    Vector2D rotate(double a);
    Vector2D& operator= (const Vector2D& v);
    string toString() const;

    friend Vector2D operator+ (const Vector2D& v, const Vector2D& v2);
    friend Vector2D operator- (const Vector2D& v, const Vector2D& v2);
    friend Vector2D operator* (const Vector2D& v, const Vector2D& v2);
    friend Vector2D operator/ (const Vector2D& v, const Vector2D& v2);
    friend Vector2D operator* (double v, const Vector2D& v2);
    friend Vector2D operator/ (double v, const Vector2D& v2);
    friend Vector2D operator* (const Vector2D& v, double v2);
    friend Vector2D operator/ (const Vector2D& v, double v2);
};

Vector2D operator+ (const Vector2D& v, const Vector2D& v2);
Vector2D operator- (const Vector2D& v, const Vector2D& v2);
Vector2D operator* (const Vector2D& v, const Vector2D& v2);
Vector2D operator/ (const Vector2D& v, const Vector2D& v2);
Vector2D operator* (double v, const Vector2D& v2);
Vector2D operator/ (double v, const Vector2D& v2);
Vector2D operator* (const Vector2D& v, double v2);
Vector2D operator/ (const Vector2D& v, double v2);

ostream& operator<< (ostream& o, const Vector2D& v);
