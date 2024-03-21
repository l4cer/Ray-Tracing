#ifndef CAMERA_H
#define CAMERA_H

#include "utils.hpp"

#include "hittable.hpp"
#include "material.hpp"
#include <omp.h>

class Camera {
private:
    int m_width;
    int m_height;

    point m_position;

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
        constructor(t_width, t_height, vector(2.0, 0.0, 0.5), 1.6 / t_width);
    }

    Camera(int t_width, int t_height, double t_viewport_ratio) {
        constructor(t_width, t_height, vector(2.0, 0.0, 0.5), t_viewport_ratio);
    }

    Camera(int t_width, int t_height, const point &t_position, double t_viewport_ratio) {
        constructor(t_width, t_height, t_position, t_viewport_ratio);
    }

    void constructor(int t_width, int t_height, const point &t_position, double t_viewport_ratio) {
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
        if (depth <= 0)
            return color(0.0, 0.0, 0.0);

        HitInfo info;

        if (!world.hit(ray, info))
            return color(0.0, 0.0, 0.0);

        color emitted = info.material->emitted(info);

        Ray scattered;
        color attenuation;

        // Recursive ray scattering
        if (info.material->scatter(ray, info, attenuation, scattered))
            return emitted + attenuation * rayColor(scattered, world, depth-1);

        return emitted;
    }

    void render(ImageHandler &handler, const HittableList &world) {
        int i, j;

        color * pixels = new color[m_width * m_height];

        int num_threads = omp_get_num_procs();  // Gets total num of threads
        omp_set_dynamic(0);                     // Sets num of max threds used in parallel block
        omp_set_num_threads(num_threads);       // Sets num of threads used in a parallel block

        #pragma omp parallel for private(i) schedule(dynamic, 10)
        for (j = 0; j < m_height; j++) {
            for (i = 0; i < m_width; i++) {
                color pixel_color = color(0.0, 0.0, 0.0);

                // Anti-aliasing sampling
                for (int sample = 0; sample < aa_sampling; sample++) {
                    vector pixel_pos = m_viewport_anchor;

                    pixel_pos += (i + random_double() - 0.5) * m_delta_u;
                    pixel_pos += (j + random_double() - 0.5) * m_delta_v;

                    Ray ray = Ray(pixel_pos, pixel_pos - m_position);
                    pixel_color += rayColor(ray, world, max_depth);
                }

                pixels[j * m_width + i] =
                    color(
                        // Gamma correction and anti-aliasing sampling
                        std::sqrt(pixel_color[0] / aa_sampling),
                        std::sqrt(pixel_color[1] / aa_sampling),
                        std::sqrt(pixel_color[2] / aa_sampling)
                    )
                ;
            }
        }

        for (j = 0; j < m_height; j++) {
            for (i = 0; i < m_width; i++)
                handler.putPixel(pixels[j * m_width + i]);
        }
    }

    ~Camera() = default;
};

#endif
