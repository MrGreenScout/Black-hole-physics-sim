#ifndef RENDERER_MATH_VECTOR3_
#define RENDERER_MATH_VECTOR3_

#include <cmath>
#include <ostream>

#define BLACK Color(0, 0, 0)
#define WHITE Color(1, 1, 1)
#define RED Color(1, 0, 0)
#define GREEN Color(1, 0, 0)
#define BLUE Color(0, 0, 1)
#define YELLOW Color (1, 1, 0)
#define PURPLE Color(1, 0, 1)
#define GRAY Color(0.5,0.5,0.5)

/**
 * Vector3 represents a 3 dimensional vector
 */
class Vector3
{
private:
    double content[3];

public:
    Vector3() : content{0, 0, 0} {}
    Vector3(double x, double y, double z) : content{x, y, z} {}

    double x() const { return content[0]; }
    double y() const { return content[1]; }
    double z() const { return content[2]; }

    Vector3 operator-() const { return Vector3(-content[0], -content[1], -content[2]); }
    double operator[](int i) const { return content[i]; }
    double &operator[](int i) { return content[i]; }

    Vector3 &operator+=(const Vector3 &other)
    {
        content[0] += other.content[0];
        content[1] += other.content[1];
        content[2] += other.content[2];

        return *this;
    }

    Vector3 &operator*=(const double scalar)
    {
        content[0] *= scalar;
        content[1] *= scalar;
        content[2] *= scalar;

        return *this;
    }

    Vector3 &operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const
    {
        return content[0] * content[0] + content[1] * content[1] + content[2] * content[2];
    }

    std::string toString() const
    {
        return "Vector3("
            + std::to_string(this->x()) + ", "  
            + std::to_string(this->y()) + ", " 
            + std::to_string(this->z()) + ')';
    }

    friend std::ostream &operator<<(std::ostream &out, const Vector3 &v);
    friend Vector3 operator+(const Vector3 &u, const Vector3 &v);
    friend Vector3 operator-(const Vector3 &u, const Vector3 &v);
    friend Vector3 operator*(const Vector3 &u, const Vector3 &v);
    friend Vector3 operator*(double t, const Vector3 &v);
    friend double dot(const Vector3 &u, const Vector3 &v);
    friend Vector3 cross(const Vector3 &u, const Vector3 &v);
    friend Vector3 mix(const Vector3 &u, const Vector3 &v, double d);
    friend Vector3 pow(const Vector3 &u, double d);
};

using Point3 = Vector3;
using EulerAngle3 = Vector3;
using Color = Vector3;

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v)
{
    return out << v.content[0] << ' ' << v.content[1] << ' ' << v.content[2];
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.content[0] + v.content[0], u.content[1] + v.content[1], u.content[2] + v.content[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.content[0] - v.content[0], u.content[1] - v.content[1], u.content[2] - v.content[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.content[0] * v.content[0], u.content[1] * v.content[1], u.content[2] * v.content[2]);
}

inline Vector3 operator*(double t, const Vector3 &v)
{
    return Vector3(t * v.content[0], t * v.content[1], t * v.content[2]);
}

inline Vector3 operator*(const Vector3 &v, double t)
{
    return t * v;
}

inline Vector3 operator/(const Vector3 &v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v)
{
    return u.content[0] * v.content[0] + u.content[1] * v.content[1] + u.content[2] * v.content[2];
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.content[1] * v.content[2] - u.content[2] * v.content[1],
                   u.content[2] * v.content[0] - u.content[0] * v.content[2],
                   u.content[0] * v.content[1] - u.content[1] * v.content[0]);
}

inline Vector3 normalize(const Vector3 &v)
{
    return v / v.length();
}

/**
 * Lerp between two vectors via a delta in the range [0,1]
 */
inline Vector3 mix(const Vector3 &u, const Vector3 &v, double d)
{
    return (u * (1 - d)) + (v * d);
}

/**
 * Component-wise power function
 */
inline Vector3 pow(const Vector3 &u, double d)
{
    return Vector3(std::pow(u.x(),d), std::pow(u.y(),d), std::pow(u.y(),d));
}
/**
 * Projects the vector a on the vector b
 */
inline Vector3 project(Vector3 a, Vector3 b)
{
    return (dot(a,b) / dot(b,b)) * b;
}

#endif