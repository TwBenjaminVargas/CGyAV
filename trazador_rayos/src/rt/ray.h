#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

/// Rayo P(t) = o + t d, con d normalizada
class Ray {
public:
    Ray(void) : origin_(0.0f), direction_(0.0f, 0.0f, 1.0f) {}
    Ray(const glm::vec3& origin, const glm::vec3& direction) :
        origin_(origin), direction_(direction) {}

    const glm::vec3& o(void) const  { return origin_; }
    const glm::vec3& d(void) const  { return direction_; }

    glm::vec3 at(float t) const     { return origin_ + t * direction_; }

private:
    glm::vec3 origin_;
    glm::vec3 direction_;
};

#endif // RAY_H
