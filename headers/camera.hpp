#ifndef CAMERA_H
#define CAMERA_H

#include "utils.hpp"

#include "hittable.hpp"
#include "material.hpp"


class Camera {
private:
    int m_width;
    int m_height;

    vector m_position;

    vector m_delta_u;
    vector m_delta_v;

    vector m_viewport_u;
    vector m_viewport_v;
    vector m_viewport_anchor;

    // Anti-aliasing sampling
    int aa_sampling = 100;

    // Recursive ray scattering
    int max_depth = 10;

public:
    Camera() {
        constructor(400, 225, vector(2.0, 0.0, 0.5), 0.004);
    }

    Camera(int t_width, int t_height) {
        constructor(t_width, t_height, vector(2.0, 0.0, 0.5), 0.004);
    }

    Camera(int t_width, int t_height, double t_viewport_ratio) {
        constructor(t_width, t_height, vector(2.0, 0.0, 0.5), t_viewport_ratio);
    }

    Camera(int t_width, int t_height, const vector &t_position, double t_viewport_ratio) {
        constructor(t_width, t_height, t_position, t_viewport_ratio);
    }

    void constructor(int t_width, int t_height, const vector &t_position, double t_viewport_ratio) {
        m_width = t_width;
        m_height = t_height;

        m_position = t_position;

        m_delta_u = vector(0.0, t_viewport_ratio, 0.0);
        m_delta_v = vector(0.0, 0.0, -t_viewport_ratio);

        m_viewport_u = m_width * m_delta_u;
        m_viewport_v = m_height * m_delta_v;

        double focal_length = 1.0;
        m_viewport_anchor = m_position - vector(focal_length, 0.0, 0.0);
        m_viewport_anchor -= 0.5 * (m_viewport_u + m_viewport_v);
    }

    color rayColor(const Ray &ray, const HittableList &world, int depth) {
        HitInfo info;

        if (depth < 0) return color(0.0, 0.0, 0.0);

        if (world.hit(ray, info)) {
            Ray scattered;
            color attenuation;

            // Recursive ray scattering
            if (info.material->scatter(ray, info, attenuation, scattered))
                return attenuation * rayColor(scattered, world, depth-1);

            return attenuation;
        }

        return color(0.0, 0.0, 0.0);
    }

    void render(ImageHandler &handler, const HittableList &world) {
        for (int j = 0; j < m_height; j++) {
            for (int i = 0; i < m_width; i++) {
                color pixel_color = color(0.0, 0.0, 0.0);

                // Anti-aliasing sampling
                for (int sample = 0; sample < aa_sampling; sample++) {
                    vector pixel_pos = m_viewport_anchor;

                    pixel_pos += (i + random_double() - 0.5) * m_delta_u;
                    pixel_pos += (j + random_double() - 0.5) * m_delta_v;

                    Ray ray = Ray(pixel_pos, pixel_pos - m_position);
                    pixel_color += rayColor(ray, world, max_depth);
                }

                handler.putPixel(
                    color(
                        // Gamma correction and anti-aliasing sampling
                        std::sqrt(pixel_color[0] / aa_sampling),
                        std::sqrt(pixel_color[1] / aa_sampling),
                        std::sqrt(pixel_color[2] / aa_sampling)
                    )
                );
            }
        }
    }

    ~Camera() = default;
};

#endif
