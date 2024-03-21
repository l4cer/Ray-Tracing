#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>


inline float quake_sqrt(float number){
    long i;

    float x = number * 0.5;
    float y = number;

    const float threehalfs = 1.5f;

    i = *(long *) &y;  // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // what the ****?
    y = *(float *) &i;
    y = y * (threehalfs - (x * y * y));  // 1st iteration
    y = y * (threehalfs - (x * y * y));  // 2nd iteration

    return y;
}

class vector {
public:
    double e[3];

    vector() : e{0.0, 0.0, 0.0} {}

    vector(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    vector operator-() const { return vector(-e[0], -e[1], -e[2]); }

    vector &operator+=(const vector &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vector &operator-=(const vector &v) {
        return *this += -v;
    }

    vector &operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    vector &operator/=(double t) {
        return *this *= 1.0 / t;
    }

    double squared_norm() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double norm() const {
        return std::sqrt(squared_norm());
    }

    bool near_zero() const {
        double threshold = 1e-8;

        return e[0] <= threshold && e[1] <= threshold && e[2] <= threshold;
    }

    ~vector() = default;
};


using point = vector;


inline std::ostream &operator<<(std::ostream &out, const vector &v) {
    return out << v.x() << " " << v.y() << " " << v.z();
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

inline vector operator/(const vector &v, double t) {
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

inline vector normalize(const vector &v) {
    return v * quake_sqrt(v.squared_norm());
}

inline vector lerp(const vector &u, const vector &v, double t) {
    return (1.0 - t) * u + t * v;
}

#endif
