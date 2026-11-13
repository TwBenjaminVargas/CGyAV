#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

/// Modelo de luz puntual. El factor de atenuación con la distancia d es
///     f_att = 1 / (c0 + c1 d + c2 d^2)
/// Por defecto vale 1 (sin atenuación)
class Light {
public:
    Light(const glm::vec3& pos, const glm::vec3& color,
          const glm::vec3& att = glm::vec3(1.0f, 0.0f, 0.0f)) :
        pos_(pos), color_(color), att_(att) {}

    const glm::vec3& pos(void) const    { return pos_; }
    const glm::vec3& color(void) const  { return color_; }

    float attenuation(float d) const {
        return 1.0f / (att_.x + att_.y * d + att_.z * d * d);
    }

private:
    glm::vec3 pos_;
    glm::vec3 color_;
    glm::vec3 att_;     ///< (c0, c1, c2) coeficientes del modelo de atenuación
};

#endif // LIGHT_H
