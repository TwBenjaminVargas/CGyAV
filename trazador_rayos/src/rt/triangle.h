#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <object.h>

/// Triángulo (Möller-Trumbore). La normal sale de (b - a) x (c - a)
class Triangle : public Object {
public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
             std::shared_ptr<Material> m) :
        Object(std::move(m)), v0_(a), e1_(b - a), e2_(c - a),
        n_(glm::normalize(glm::cross(e1_, e2_))) {}

    bool intersect(const Ray& r, Hit& out) const override {
        const glm::vec3 h = glm::cross(r.d(), e2_);
        const float a = glm::dot(e1_, h);
        if( glm::abs(a) < 1e-7f ) return false;        // rayo paralelo al triángulo
        const float f = 1.0f / a;

        // Coordenadas baricéntricas (u, v) del punto de intersección
        const glm::vec3 s = r.o() - v0_;
        const float u = f * glm::dot(s, h);
        if( u < 0.0f || u > 1.0f ) return false;
        const glm::vec3 q = glm::cross(s, e1_);
        const float v = f * glm::dot(r.d(), q);
        if( v < 0.0f || u + v > 1.0f ) return false;

        const float t = f * glm::dot(e2_, q);
        if( t <= EPSILON ) return false;

        out.t   = t;
        out.p   = r.at(t);
        out.mat = mat_.get();
        out.set_face(r, n_);
        return true;
    }

private:
    glm::vec3 v0_, e1_, e2_;
    glm::vec3 n_;
};

#endif // TRIANGLE_H
