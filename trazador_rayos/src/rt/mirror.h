#ifndef MIRROR_H
#define MIRROR_H

#include <material.h>

/// Espejo o metal opaco: Phong completo más un rayo reflejado de peso kr
/// constante.  I = I_local + kr * I(R)
/// El brillo de Phong sigue haciendo falta: una luz puntual no es geometría, el
/// rayo reflejado nunca la encuentra
class Mirror : public Material {
public:
    float kr = 0.8f;

    int secondary(const Ray& in, const Hit& h, Secondary out[2]) const override {
        // El origen se corre un poco hacia afuera para no chocar con la propia
        // superficie por redondeo
        out[0].ray    = Ray(h.p + h.n * EPSILON, glm::reflect(glm::normalize(in.d()), h.n));
        out[0].weight = kr;
        return 1;
    }
};

#endif // MIRROR_H
