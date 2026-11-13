#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include <material.h>

/// Dieléctrico (vidrio, agua): un rayo reflejado y uno refractado, repartidos
/// según Fresnel con la aproximación de Schlick.
///     I = brillo + F(θ) * I(R) + (1 - F(θ)) * I(T)
/// ka y kd no intervienen; ks y shininess dan el brillo de las luces.
///
/// ior es el índice del lado de adentro relativo al de afuera (la normal
/// geométrica apunta hacia afuera). En una esfera de vidrio maciza es 1.5; en
/// la superficie interior de una esfera hueca, 1/1.5 (de vidrio a aire).
class Dielectric : public Material {
public:
    float ior = 1.5f;
    /// Fracción de luz que pasa por cada superficie que cruza un rayo de sombra
    /// (el vidrio hace una sombra más clara)
    float transparency = 0.8f;

    Dielectric(void) { ka = glm::vec3(0.0f); kd = glm::vec3(0.0f); ks = glm::vec3(1.0f); shininess = 100.0f; }

    /// Sólo el brillo, y sólo si el rayo llega desde afuera: desde adentro la
    /// luz llegaría refractada por la superficie, pero el rayo de sombra va
    /// derecho a la luz y el brillo quedaría en el lugar equivocado
    glm::vec3 local(const Scene& scene, const Ray& r, const Hit& h) const override;

    int secondary(const Ray& in, const Hit& h, Secondary out[2]) const override;

    float shadow_transmission(void) const override { return transparency; }
};

#endif // DIELECTRIC_H
