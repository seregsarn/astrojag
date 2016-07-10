#include "astrojag_internal.h"

namespace astrojag {

const Vector Vector::origin(0.0f,0.0f);

Vector::Vector() : x(0.0f), y(0.0f) {}
Vector::Vector(float _x, float _y) : x(_x), y(_y) {}
Vector::Vector(const Vector& other) : x(other.x), y(other.y) {}
Vector::Vector(const Point& p) : x(p.x), y(p.y) {}

Vector& Vector::operator=(const Vector& other) {
    x = other.x;
    y = other.y;
    return *this;
}
Vector& Vector::operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector& Vector::operator-=(const Vector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector& Vector::operator*=(const float s) {
    x *= s;
    y *= s;
    return *this;
}
Vector& Vector::operator/=(const float s) {
    x /= s;
    y /= s;
    return *this;
}

float Vector::length() const {
    return sqrt(((x*x) + (y*y)));
}
float Vector::lengthSquared() const {
    return ((x*x) + (y*y));
}
float Vector::dot(const Vector& a, const Vector& b) {
    return ((a.x*b.x) + (a.y*b.y));
}

bool operator==(const Vector& a, const Vector& b) {
    return (a.x == b.x && a.y == b.y);
}
bool operator!=(const Vector& a, const Vector& b) {
    return (a.x != b.x || a.y != b.y);
}
bool operator<(const Vector& a, const Vector& b) {
    return (a.x < b.x || a.y < b.y);
}
Vector operator+(const Vector& a, const Vector& b) {
    return Vector(a.x+b.x, a.y+b.y);
}
Vector operator-(const Vector& a, const Vector& b) {
    return Vector(a.x-b.x, a.y-b.y);
}
Vector operator*(const Vector& a, float b) {
    return Vector(a.x*b, a.y*b);
}
Vector operator/(const Vector& a, float b) {
    return Vector(a.x/b, a.y/b);
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
   return os << '(' << v.x << ',' << v.y << ')';
}

//=============================================================

Point::Point() : x(0), y(0) {}
Point::Point(int32_t _x, int32_t _y) : x(_x), y(_y) {}
Point::Point(const Point& other) : x(other.x), y(other.y) {}
Point::Point(const Vector& v) : x((int)v.x), y((int)v.y) {}
Point& Point::operator=(const Point& other) {
    x = other.x;
    y = other.y;
    return *this;
}
Point& Point::operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Point& Point::operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
float Point::distance(const Point& other) const {
    Vector v(abs(other.x - x), abs(other.y - y));
    return v.length();
}
int Point::manhattan(const Point& other) const {
    Point p(abs(other.x - x), abs(other.y - y));
    return (p.x + p.y);    
}
int Point::distanceSquared(const Point& other) const {
    Point p(abs(other.x - x), abs(other.y - y));
    return ((p.x*p.x) + (p.y*p.y));
}

const Point Point::origin(0,0);

bool operator==(const Point& a, const Point& b) {
    return (a.x == b.x && a.y == b.y);
}
bool operator!=(const Point& a, const Point& b) {
    return (a.x != b.x || a.y != b.y);
}
bool operator<(const Point& a, const Point& b) {
    return (a.x < b.x ? true : (a.y < b.y ? true : false));
}
Point operator+(const Point& a, const Point& b) {
    return Point(a.x+b.x, a.y+b.y);
}
Point operator-(const Point& a, const Point& b) {
    return Point(a.x-b.x, a.y-b.y);
}
Point operator*(const Point& a, int32_t b) {
    return Point(a.x*b, a.y*b);
}
Point operator/(const Point& a, int32_t b) {
    return Point(a.x/b, a.y/b);
}

std::ostream& operator<<(std::ostream& os, const Point& b)
{
   return os << '(' << b.x << ',' << b.y << ')';
}

}; // namespace astrojag