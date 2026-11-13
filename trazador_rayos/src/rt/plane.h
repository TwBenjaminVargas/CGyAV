#ifndef PLANE_H
#define PLANE_H

#include <object.h>

/// Plano infinito, modelado como un punto y una normal
class Plane : public Object {
public:
    Plane(const glm::vec3& point, const glm::vec3& normal, std::shared_ptr<Material> m) :
        Object(std::move(m)), p0_(point), n_(glm::normalize(normal)) {}

    bool intersect(const Ray& r, Hit& out) const override {
        // (p0 - (o + t d)) . n = 0   ->   t = (p0 - o) . n / (d . n)
        const float denom = glm::dot(n_, r.d());
        if( glm::abs(denom) < 1e-7f ) return false;    // rayo paralelo al plano
        const float t = glm::dot(p0_ - r.o(), n_) / denom;
        if( t <= EPSILON ) return false;

        out.t   = t;
        out.p   = r.at(t);
        out.mat = mat_.get();
        out.set_face(r, n_);
        return true;
    }

private:
    glm::vec3 p0_;
    glm::vec3 n_;
};

#endif // PLANE_H
