// Vector and Point
//==========================================================
// basic types for representing points and vectors. These are used
// throughout by most of the algorithms.

class Point;
class Vector;

//-------------------------------------------------
// 2D float vectors.
class Vector {
public:
    float x, y;
    Vector();
    Vector(float x, float y);
    Vector(const Vector& other);
    Vector(const Point& p);
    Vector& operator=(const Vector& other);
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(const float s);
    Vector& operator/=(const float s);
    float length() const;
    float lengthSquared() const;
    static float dot(const Vector& a, const Vector& b);
    static const Vector origin;
};

bool operator==(const Vector& a, const Vector& b);
bool operator!=(const Vector& a, const Vector& b);
bool operator<(const Vector& a, const Vector& b);
Vector operator+(const Vector& a, const Vector& b);
Vector operator-(const Vector& a, const Vector& b);

Vector operator*(const Vector& a, float b);
Vector operator/(const Vector& a, float b);

std::ostream& operator<<(std::ostream& os, const Vector& b);

//-------------------------------------------------
// 2D int vectors.
class Point {
public:
    int32_t x, y;
    Point();
    Point(int32_t x, int32_t y);
    Point(const Point& other);
    Point(const Vector& v);
    Point& operator=(const Point& other);
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
    float distance(const Point& other) const;
    int manhattan(const Point& other) const;
    int distanceSquared(const Point& other) const;
    static const Point origin;
};

bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);
Point operator+(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
Point operator*(const Point& a, int32_t b);
Point operator/(const Point& a, int32_t b);

std::ostream& operator<<(std::ostream& os, const Point& b);
