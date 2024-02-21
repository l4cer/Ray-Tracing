#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <iostream>

#include "vector.hpp"


int clamp(int value) {
    return (value < 0) ? 0 : ((value > 255) ? 255 : value);
}


class color: public vector {
public:
    color() : vector() {}

    color(double e0, double e1, double e2) : vector(e0, e1, e2) {}

    color(const vector &v) : vector(v.e[0], v.e[1], v.e[2]) {}

    int r() const { return clamp(static_cast<int>(255.999 * e[0])); }
    int g() const { return clamp(static_cast<int>(255.999 * e[1])); }
    int b() const { return clamp(static_cast<int>(255.999 * e[2])); }

    ~color() = default;
};


inline std::ostream &operator<<(std::ostream &out, const color &c) {
    return out << c.r() << " " << c.g() << " " << c.b();
}

#endif
