#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <cmath>
#include <iostream>

using std::sqrt;


class vector {
public:
    double e[3];

    vector() : e{0.0, 0.0, 0.0} {}
    vector(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    int r() const { return static_cast<int>(255.999 * e[0]); }
    int g() const { return static_cast<int>(255.999 * e[1]); }
    int b() const { return static_cast<int>(255.999 * e[2]); }

    vector operator-() const { return vector(-e[0], -e[1], -e[2]); }

    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vector& operator+=(const vector &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vector& operator-=(const vector &v) {
        return *this += -v;
    }

    vector& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    vector& operator/=(double t) {
        return *this *= 1.0 / t;
    }

    double squared_norm() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double norm() const {
        return sqrt(squared_norm());
    }
};

// point is just an alias for vector, but useful for geometric clarity
using point = vector;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vector &v) {
    return out << v.r() << " " << v.g() << " " << v.b();
}

inline vector operator+(const vector &u, const vector &v) {
    return vector(
        u.e[0] + v.e[0],
        u.e[1] + v.e[1],
        u.e[2] + v.e[2]);
}

inline vector operator-(const vector &u, const vector &v) {
    return vector(
        u.e[0] - v.e[0],
        u.e[1] - v.e[1],
        u.e[2] - v.e[2]);
}

inline vector operator*(const vector &u, const vector &v) {
    return vector(
        u.e[0] * v.e[0],
        u.e[1] * v.e[1],
        u.e[2] * v.e[2]);
}

inline vector operator*(double t, const vector &v) {
    return vector(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vector operator*(const vector &v, double t) {
    return t * v;
}

inline vector operator/(vector v, double t) {
    return (1.0 / t) * v;
}

inline double dot(const vector &u, const vector &v) {
    return (
        u.e[0] * v.e[0] +
        u.e[1] * v.e[1] +
        u.e[2] * v.e[2]);
}

inline vector cross(const vector &u, const vector &v) {
    return vector(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vector normalize(vector v) {
    return v / v.norm();
}

#endif
