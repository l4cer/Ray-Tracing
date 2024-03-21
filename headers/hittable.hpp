#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <vector>

#include "ray.hpp"


class Material;


struct HitInfo {
    double root;
    point hit_point;
    vector normal;
    std::shared_ptr<Material> material;
    double texture_u;
    double texture_v;
};


class Hittable {
private:
    std::shared_ptr<Material> m_material;

public:
    Hittable() : m_material(std::shared_ptr<Material>()) {}

    Hittable(std::shared_ptr<Material> t_material) {
        m_material = t_material;
    }

    std::shared_ptr<Material> getMaterial() const { return m_material; }

    virtual bool hit(const Ray &ray, HitInfo &info) const = 0;

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

    Sphere(point t_center, double t_radius, std::shared_ptr<Material> t_material) : Hittable(t_material) {
        m_center = t_center;
        m_radius = t_radius;
    }

    point getCenter() const { return m_center; }

    double getRadius() const { return m_radius; }

    bool hit(const Ray &ray, HitInfo &info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        // Roots of x² + 2bx + c = 0
        double b = dot(direction, origin - m_center);
        double c = (origin - m_center).squared_norm() - m_radius * m_radius;

        double delta = b * b - c;
        if (delta < 0.0) return false;

        double sqrt_delta = sqrt(delta);
        double root = -b - sqrt_delta;

        if (root < 0.001) {
            root = -b + sqrt_delta;
            if (root < 0.001) return false;
        }

        info.root = root;
        info.hit_point = ray.at(root);
        info.normal = normalize(info.hit_point - m_center);
        info.material = getMaterial();

        info.texture_u = atan2(info.normal.y(), info.normal.x()) / tau + 0.5;
        info.texture_v = acos(info.normal.z()) / pi;

        return true;
    }

    ~Sphere() = default;
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

    Plane(point t_point, vector t_normal, std::shared_ptr<Material> t_material) : Hittable(t_material) {
        m_point = t_point;
        m_normal = normalize(t_normal);
    }

    point getPoint() const { return m_point; }

    vector getNormal() const { return m_normal; }

    bool hit(const Ray &ray, HitInfo &info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        double den = dot(direction, m_normal);

        if (den == 0.0) return false;

        double num = dot(m_point - origin, m_normal);
        double root = num / den;

        if (root < 0.001) return false;

        info.root = root;
        info.hit_point = ray.at(root);
        info.normal = m_normal;
        info.material = getMaterial();

        double tmp;   // Discard the integer part
        info.texture_u = std::modf(0.25 * info.hit_point.x(), &tmp);
        info.texture_v = std::modf(0.25 * info.hit_point.y(), &tmp);

        return true;
    }

    ~Plane() = default;
};


class Quad: public Hittable {
private:
    point m_point;
    vector m_vector_u;
    vector m_vector_v;
    vector m_normal;

public:
    Quad() : Hittable() {
        m_point = point(0.0, 0.0, 0.0);
        m_vector_u = vector(1.0, 0.0, 0.0);
        m_vector_v = vector(0.0, 1.0, 0.0);
        m_normal = cross(m_vector_u, m_vector_v);
    }

    Quad(point t_point, vector t_vector_u, vector t_vector_v, std::shared_ptr<Material> t_material) : Hittable(t_material) {
        m_point = t_point;
        m_vector_u = t_vector_u;
        m_vector_v = t_vector_v;
        m_normal = cross(m_vector_u, m_vector_v);
    }

    point getPoint() const { return m_point; }

    vector getVectorU() const { return m_vector_u; }

    vector getVectorV() const { return m_vector_v; }

    vector getNormal() const { return m_normal; }

    bool hit(const Ray &ray, HitInfo &info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        double den = dot(direction, m_normal);

        if (den == 0.0) return false;

        double num = dot(m_point - origin, m_normal);
        double root = num / den;

        if (root < 0.001) return false;

        vector delta = ray.at(root) - m_point;

        float u_pos = dot(delta, m_vector_u) / m_vector_u.squared_norm();
        float v_pos = dot(delta, m_vector_v) / m_vector_v.squared_norm();

        if (u_pos < 0 || 1 < u_pos || v_pos < 0 || 1 < v_pos) return false;

        info.root = root;
        info.hit_point = ray.at(root);
        info.normal = m_normal;
        info.material = getMaterial();

        info.texture_u = u_pos;
        info.texture_v = v_pos;

        return true;
    }

    ~Quad() = default;
};


class Box: public Hittable {
private:
    point m_center;
    vector m_sizes;

    std::vector<std::shared_ptr<Quad>> m_faces;

public:
    Box() : Hittable() {
        m_center = point(0.0, 0.0, 0.0);
        m_sizes = vector(1.0, 1.0, 1.0);

        createFaces();
    }

    Box(point t_center, vector t_sizes, std::shared_ptr<Material> t_material) : Hittable(t_material) {
        m_center = t_center;
        m_sizes = t_sizes;

        createFaces();
    }

    void createFaces() {
        vector point_A = m_center + 0.5 * m_sizes;
        vector point_B = m_center - 0.5 * m_sizes;

        vector x = vector(m_sizes[0] * 1.0, 0.0, 0.0);
        vector y = vector(0.0, m_sizes[1] * 1.0, 0.0);
        vector z = vector(0.0, 0.0, m_sizes[2] * 1.0);

        m_faces.push_back(std::make_shared<Quad>(point_A, -x, -y, getMaterial()));
        m_faces.push_back(std::make_shared<Quad>(point_A, -y, -z, getMaterial()));
        m_faces.push_back(std::make_shared<Quad>(point_A, -z, -x, getMaterial()));
        m_faces.push_back(std::make_shared<Quad>(point_B,  y,  x, getMaterial()));
        m_faces.push_back(std::make_shared<Quad>(point_B,  z,  y, getMaterial()));
        m_faces.push_back(std::make_shared<Quad>(point_B,  x,  z, getMaterial()));
    }

    point getCenter() const { return m_center; }

    vector getSizes() const { return m_sizes; }

    bool hit(const Ray &ray, HitInfo &info) const override {
        point origin = ray.getOrigin();
        vector direction = ray.getDirection();

        int index;
        HitInfo curr;

        info.root = -1.0;

        for (int i = 0; i < 6; i++) {
            if (m_faces[i]->hit(ray, curr)) {
                if (info.root == -1 || curr.root < info.root) {
                    index = i;
                    info = curr;
                }
            }
        }

        if (info.root == -1.0) return false;

        double delta_u;
        double delta_v;

        switch (index) {
            case 0: delta_u = 1; delta_v = 0; break;
            case 1: delta_u = 1; delta_v = 1; break;
            case 2: delta_u = 2; delta_v = 1; break;
            case 3: delta_u = 1; delta_v = 2; break;
            case 4: delta_u = 1; delta_v = 3; break;
            case 5: delta_u = 0; delta_v = 1; break;
            default: break;
        }

        info.texture_u = (info.texture_u + delta_u) / 4.0;
        info.texture_v = (info.texture_v + delta_v) / 4.0;

        return true;
    }

    ~Box() = default;
};


class HittableList: public Hittable {
private:
    std::vector<std::shared_ptr<Hittable>> m_objects;

public:
    HittableList() {}

    HittableList(std::shared_ptr<Hittable> t_object) { add(t_object); }

    std::vector<std::shared_ptr<Hittable>> getObjects() const { return m_objects; }

    void clear() { m_objects.clear(); }

    void add(std::shared_ptr<Hittable> t_object) {
        m_objects.push_back(t_object);
    }

    bool hit(const Ray &ray, HitInfo &info) const override {
        HitInfo tmp_info;

        // Negative value to indicate the non-hit
        double root = -1.0;

        for (const std::shared_ptr<Hittable> object: m_objects) {
            if (object->hit(ray, tmp_info)) {
                if (root == -1.0 || tmp_info.root < root) {
                    root = tmp_info.root;
                    info = tmp_info;
                }
            }
        }

        return root != -1.0;
    }

    ~HittableList() = default;
};

#endif
