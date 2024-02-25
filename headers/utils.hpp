#ifndef UTILS_H
#define UTILS_H

#include <limits>
#include <random>


const double pi = 3.141592653589;


inline double deg2rad(double deg) {
    return deg * pi / 180.0;
};

inline double rad2deg(double rad) {
    return rad * 180.0 / pi;
};

inline double random_double() {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
};

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
};

inline vector random_vector() {
    return vector(
        random_double(),
        random_double(),
        random_double()
    );
};

inline vector random_vector(double min, double max) {
    return vector(
        random_double(min, max),
        random_double(min, max),
        random_double(min, max)
    );
};

inline vector random_unit_vector() {
    double phi = random_double(0.0, pi);
    double theta = random_double(0.0, 2.0 * pi);

    return vector(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
}

inline vector random_on_hemisphere(const vector &normal) {
    vector unit_vector = random_unit_vector();

    return (dot(unit_vector, normal) > 0.0) ? unit_vector : -unit_vector;
}

#endif
