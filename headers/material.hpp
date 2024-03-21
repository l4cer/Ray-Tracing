#ifndef MEATERIAL_H
#define MEATERIAL_H

#include "texture.hpp"


class Material {
private:
    std::shared_ptr<Texture> m_texture;

public:
    Material() : m_texture(std::shared_ptr<SolidTexture>()) {}

    Material(std::shared_ptr<Texture> t_texture) {
        m_texture = t_texture;
    }

    std::shared_ptr<Texture> getTexture() const { return m_texture; }

    virtual bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const = 0;

    ~Material() = default;
};


class LightSource: public Material {
public:
    LightSource() : Material() {}

    LightSource(std::shared_ptr<Texture> t_texture) : Material(t_texture) {}

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        attenuation = getTexture()->getColorInTexture(
            info.texture_u, info.texture_v, info.hit_point);

        return false;
    }

    ~LightSource() = default;
};


class Lambertian: public Material {
public:
    Lambertian() : Material() {}

    Lambertian(std::shared_ptr<Texture> t_texture) : Material(t_texture) {}

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        scattered = Ray(
            info.hit_point, info.normal + random_unit_vector());
        attenuation = getTexture()->getColorInTexture(
            info.texture_u, info.texture_v, info.hit_point);

        return true;
    }

    ~Lambertian() = default;
};


class Metal: public Material {
private:
    double m_fuzzy;

public:
    Metal() : Material() {
        m_fuzzy = 0.2;
    }

    Metal(std::shared_ptr<Texture> t_texture, double t_fuzzy) : Material(t_texture) {
        m_fuzzy = t_fuzzy;
    }

    double getFuzzy() const { return m_fuzzy; }

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        vector reflected = ray.getDirection() -
            2.0 * dot(ray.getDirection(), info.normal) * info.normal;

        scattered = Ray(
            info.hit_point, reflected + m_fuzzy * random_unit_vector());
        attenuation = getTexture()->getColorInTexture(
            info.texture_u, info.texture_v, info.hit_point);

        return true;
    }

    ~Metal() = default;
};


class Dielectric: public Material {
private:
    double m_refractive_index;

    static double reflectance(double cos, double ratio) {
        // Use Schlick's approximation for reflectance
        double r0 = pow((1.0 - ratio) / (1.0 + ratio), 2);

        return r0 + (1.0 - r0) * pow(1.0 - std::fabs(cos), 5);
    }

public:
    Dielectric() : Material() {
        m_refractive_index = 1.0;
    }

    Dielectric(std::shared_ptr<Texture> t_texture, double t_refractive_index) : Material(t_texture) {
        m_refractive_index = t_refractive_index;
    }

    double getRefractiveIndex() const { return m_refractive_index; }

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        double cos = dot(ray.getDirection(), info.normal);
        double sin = std::sqrt(1.0 - cos * cos);

        double ratio = (cos < 0.0) ?  1.0 / m_refractive_index : m_refractive_index;

        if (ratio * sin > 1.0 || random_double() < reflectance(cos, ratio)) {
            vector reflected = ray.getDirection() -
                2.0 * dot(ray.getDirection(), info.normal) * info.normal;

            scattered = Ray(
                info.hit_point, reflected + random_unit_vector());

        } else {
            vector perp = ratio * (ray.getDirection() - cos * info.normal);
            vector paral = std::sqrt(1.0 - perp.squared_norm()) * info.normal;

            scattered = Ray(
                info.hit_point, (cos < 0.0) ? perp - paral : perp + paral);
        }

        attenuation = getTexture()->getColorInTexture(
            info.texture_u, info.texture_v, info.hit_point);

        return true;
    }

    ~Dielectric() = default;
};

#endif
