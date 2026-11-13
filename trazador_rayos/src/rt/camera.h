#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <ray.h>

/// Cámara sin lente (estenopeica): desde la posición del ojo sale un rayo por
/// cada punto del plano de la imagen
///
/// RAE - estenopeica: adj. Fotografía Dicho de una cámara, que tiene un orificio
/// diminuto que hace las veces de objetivo.
class Camera {
public:
    Camera(void) { look_at(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 45.0f); }

    /// fov_deg: campo de visión vertical, en grados
    void look_at(const glm::vec3& eye, const glm::vec3& target,
                 const glm::vec3& up, float fov_deg)
    {
        eye_     = eye;
        forward_ = glm::normalize(target - eye);
        right_   = glm::normalize(glm::cross(forward_, up));
        up_      = glm::cross(right_, forward_);
        scale_   = glm::tan(glm::radians(fov_deg * 0.5f));
    }

    /// Rayo primario por el píxel (i, j) de una imagen de W x H. offset es el
    /// desplazamiento dentro del píxel, en fracciones de píxel (0,0 = centro)
    Ray primary_ray(int i, int j, int W, int H, const glm::vec2& offset) const {
        const float aspect = float(W) / float(H);
        // Del píxel al plano de la imagen, a distancia 1 del ojo
        const float x = (2.0f * ((i + 0.5f + offset.x) / float(W)) - 1.0f) * aspect * scale_;
        const float y = (1.0f - 2.0f * ((j + 0.5f + offset.y) / float(H))) * scale_;
        return Ray(eye_, glm::normalize(forward_ + x * right_ + y * up_));
    }

private:
    glm::vec3 eye_, forward_, right_, up_;
    float scale_ = 1.0f;
};

#endif // CAMERA_H
