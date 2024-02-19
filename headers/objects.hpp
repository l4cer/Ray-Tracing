#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <memory>
#include <vector>

#include "ray.hpp"
#include "vector.hpp"


class HitInfo {
public:
    double root;
    point hit_point;
    vector normal;

    void setNormal(const Ray& ray, const vector& t_normal){
        bool front_hit = dot(ray.getDirection(), t_normal) < 0;

        normal = normalize(front_hit ? t_normal: -t_normal);
    }
};


class Hittable {
public:
    virtual bool hit(const Ray& ray, HitInfo& info) const = 0;

    ~Hittable() = default;
};


class Sphere: public Hittable {
private:
    point m_center;
    double m_radius;

public:
    Sphere() : Hittable() {
        m_center = point(0.0, 0.0, 0.0);
        m_radius = 1.0;
    }

    Sphere(point t_center, double t_radius) : Hittable() {
        m_center = t_center;
        m_radius = t_radius;
    }

    point getCenter() const { return m_center; }

    double getRadius() const { return m_radius; }

    bool hit(const Ray& ray, HitInfo& info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        // Roots of x² + 2bx + c = 0
        double b = dot(direction, origin - m_center);
        double c = (origin - m_center).squared_norm() - m_radius * m_radius;

        double delta = b * b - c;
        if (delta < 0.0) return false;

        double sqrt_delta = sqrt(delta);
        double root = -b - sqrt_delta;

        if (root < 0.0) {
            root = -b + sqrt_delta;
            if (root < 0.0) return false;
        }

        info.root = root;
        info.hit_point = ray.at(root);
        info.setNormal(ray, info.hit_point - m_center);

        return true;
    }
};


class Plane: public Hittable {
private:
    point m_point;
    vector m_normal;

public:
    Plane() : Hittable() {
        m_point = point(0.0, 0.0, 0.0);
        m_normal = vector(0.0, 0.0, 1.0);
    }

    Plane(point t_point, vector t_normal) : Hittable() {
        m_point = t_point;
        m_normal = normalize(t_normal);
    }

    point getPoint() const { return m_point; }

    vector getNormal() const { return m_normal; }

    bool hit(const Ray& ray, HitInfo& info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        double den = dot(direction, m_normal);

        if (den == 0.0) return false;

        double num = dot(m_point - origin, m_normal);
        double root = num / den;

        if (root < 0.0) return false;

        info.root = root;
        info.hit_point = ray.at(root);
        info.setNormal(ray, m_normal);

        return true;
    }
};


class HittableList: public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {}

    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& ray, HitInfo& info) const override {
        HitInfo tmp_info;

        // Negative value to indicate the non-hit
        double root = -1.0;

        for (const auto& object: objects) {
            if (object->hit(ray, tmp_info)) {
                if (root == -1.0 || tmp_info.root < root) {
                    root = tmp_info.root;
                    info = tmp_info;
                }
            }
        }

        return root != -1.0;
    }
};

#endif
