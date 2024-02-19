#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <memory>
#include <vector>

#include "ray.hpp"
#include "vector.hpp"


class HitInfo {
public:
    point hit_point;
    vector normal;
    double root;
    bool front_hit;

    void setNormal(const Ray& ray, const vector& t_normal){
        front_hit = dot(ray.getDirection(), t_normal) < 0;

        normal = front_hit ? t_normal: -t_normal;
    }
};


class Object {
public:
    virtual bool hit(const Ray& ray, HitInfo& info) const = 0;

    ~Object() = default;
};


class Sphere: public Object {
private:
    point m_center;
    double m_radius;

public:
    Sphere() : Object() {
        m_center = point(0.0, 0.0, 0.0);
        m_radius = 1.0;
    }

    Sphere(point t_center, double t_radius) : Object() {
        m_center = t_center;
        m_radius = t_radius;
    }

    point getCenter() const { return m_center; }

    double getRadius() const { return m_radius; }

    bool hit(const Ray& ray, HitInfo& info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        double a = direction.squared_norm();
        double b = 2.0 * dot(direction, origin - m_center);
        double c = (origin - m_center).squared_norm() - m_radius * m_radius;

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) return false;
        double sqrt_discriminant = sqrt(discriminant);

        double root = -0.5 * (sqrt_discriminant + b) / a;

        if (root < 0.0) {
            root = 0.5 * (sqrt_discriminant - b) / a;
            if (root < 0.0) return false;
        }

        info.root = root;
        info.hit_point = ray.at(root);
        info.normal = (info.hit_point - m_center) / m_radius;

        return true;
    }
};


class Plane: public Object {
private:
    point m_point;
    vector m_normal;

public:
    Plane() : Object() {
        m_point = point(0.0, 0.0, 0.0);
        m_normal = vector(0.0, 0.0, 1.0);
    }

    Plane(point t_point, vector t_normal) : Object() {
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
        info.normal = m_normal;

        return true;
    }
};


using std::shared_ptr;
using std::make_shared;


class ObjectsList: public Object {
public:
    std::vector<shared_ptr<Object>> objects;

    ObjectsList() {}

    ObjectsList(shared_ptr<Object> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Object> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& ray, HitInfo& info) const override {
        HitInfo tmp_info;

        // Negative value to indicates the non-hit
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
