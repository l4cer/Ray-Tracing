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

#endif
