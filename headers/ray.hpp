#ifndef RAY_H
#define RAY_H


class Ray {
private:
    point m_origin;
    vector m_direction;

public:
    Ray() {}

    Ray(const point &t_origin, const vector &t_direction) {
        m_origin = t_origin;
        m_direction = normalize(t_direction);
    }

    point getOrigin() const { return m_origin; }

    vector getDirection() const { return m_direction; }

    point at(double t) const {
        return m_origin + t * m_direction;
    }

    ~Ray() = default;
};

#endif
