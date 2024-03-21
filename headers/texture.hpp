#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.hpp"


class Texture {
public:
    Texture() {}

    virtual color getColorInTexture(double u, double v, const vector &t_hitpoint) const = 0;

    ~Texture() = default;
};


class SolidTexture: public Texture {
private:
    color m_color;

public:
    SolidTexture() : Texture() {
        m_color = color(0.5, 0.5, 0.5);
    }

    SolidTexture(const color &t_color) {
        m_color = t_color;
    }

    color getColor() const { return m_color; }

    color getColorInTexture(double u, double v, const vector &t_hitpoint) const override {
        return m_color;
    }

    ~SolidTexture() = default;
};


class CheckerTexture: public Texture {
private:
    color m_color_odd;
    color m_color_even;

public:
    CheckerTexture() : Texture() {
        m_color_odd = color(0.8, 0.2, 0.2);
        m_color_even = color(0.2, 0.2, 0.8);
    }

    CheckerTexture(const color &t_color_odd, const color &t_color_even) {
        m_color_odd = t_color_odd;
        m_color_even = t_color_even;
    }

    color getColorOdd() const { return m_color_odd; }

    color getColorEven() const { return m_color_even; }

    color getColorInTexture(double u, double v, const vector &t_hitpoint) const override {
        int u_tmp = static_cast<int>(std::floor(12.0 * u));
        int v_tmp = static_cast<int>(std::floor(12.0 * v));

        return ((u_tmp + v_tmp) % 2 == 0) ? m_color_even : m_color_odd;
    }

    ~CheckerTexture() = default;
};


class ImageTexture : public Texture {
private:
    Image m_image;

public:
    ImageTexture() : m_image() {}

    ImageTexture(std::string filename) : m_image(filename) {}

    color getColorInTexture(double u, double v, const vector &t_hitpoint) const override {
        int i = static_cast<int>(clamp(u) * m_image.getWidth());
        int j = static_cast<int>(clamp(v) * m_image.getHeight());

        const unsigned char * pixel = m_image.pixelData(i, j);

        return color(
            pixel[0] / 255.0,
            pixel[1] / 255.0,
            pixel[2] / 255.0
        );
    }

    ~ImageTexture() = default;
};

#endif
