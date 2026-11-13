#ifndef SPHERE_H
#define SPHERE_H

#include <object.h>

class Sphere : public Object {
public:
    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> m) :
        Object(std::move(m)), c_(center), r_(radius) {}

    bool intersect(const Ray& r, Hit& out) const override {
        // |o + t d - c|^2 = r^2   ->   a t^2 + b t + cc = 0   (c es el centro)
        const glm::vec3 oc = r.o() - c_;
        const float a  = glm::dot(r.d(), r.d());
        const float b  = 2.0f * glm::dot(oc, r.d());
        const float cc = glm::dot(oc, oc) - r_ * r_;
        const float disc = b * b - 4.0f * a * cc;
        if( disc < 0.0f ) return false;

        // La raíz más chica que esté delante del origen (si el origen está
        // adentro de la esfera, es la segunda)
        const float sq = glm::sqrt(disc);
        const float t0 = (-b - sq) / (2.0f * a);
        const float t1 = (-b + sq) / (2.0f * a);
        const float t  = (t0 > EPSILON) ? t0 : ((t1 > EPSILON) ? t1 : INF);
        if( t == INF ) return false;

        out.t   = t;
        out.p   = r.at(t);
        out.mat = mat_.get();
        out.set_face(r, glm::normalize(out.p - c_));
        return true;
    }

private:
    glm::vec3 c_;
    float r_;
};

#endif // SPHERE_H
