#ifndef MEATERIAL_H
#define MEATERIAL_H


class HitInfo;


class Material {
private:
    color m_albedo;

public:
    Material() : m_albedo(color(0.5, 0.5, 0.5)) {}

    Material(const color &t_albedo) {
        m_albedo = t_albedo;
    }

    color getAlbedo() const { return m_albedo; }

    virtual bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const = 0;

    ~Material() = default;
};


class LightSource: public Material {
public:
    LightSource() : Material() {}

    LightSource(const color &t_albedo) : Material(t_albedo) {}

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        attenuation = getAlbedo();

        return false;
    }

    ~LightSource() = default;
};


class Lambertian: public Material {
public:
    Lambertian() : Material() {}

    Lambertian(const color &t_albedo) : Material(t_albedo) {}

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        scattered = Ray(
            info.hit_point, info.normal + random_unit_vector());
        attenuation = getAlbedo();

        return true;
    }

    ~Lambertian() = default;
};


class Metal: public Material {
private:
    double m_fuzzy;

public:
    Metal() : Material(), m_fuzzy(0.3) {}

    Metal(const color &t_albedo, double t_fuzzy) : Material(t_albedo) {
        m_fuzzy = t_fuzzy;
    }

    double getFuzzy() const { return m_fuzzy; }

    bool scatter(const Ray &ray, HitInfo &info, color &attenuation, Ray &scattered) const override {
        vector reflected = ray.getDirection() -
            2.0 * dot(ray.getDirection(), info.normal) * info.normal;

        scattered = Ray(
            info.hit_point, reflected + m_fuzzy * random_unit_vector());
        attenuation = getAlbedo();

        return true;
    }

    ~Metal() = default;
};

#endif
