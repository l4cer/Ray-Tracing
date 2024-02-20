#ifndef MEATERIAL_HEADER
#define MEATERIAL_HEADER

#include "color.hpp"
#include "ray.hpp"


class Material {
private:
    color m_color;
    double m_reflectance;

public:
    Material() : m_color(color(1.0, 0.0, 0.0)), m_reflectance(0.5) {}

    Material(const color &t_color, double t_reflectance) {
        m_color = t_color;
        m_reflectance = t_reflectance;
    }
};

#endif
