#include <dielectric.h>
#include <scene.h>

glm::vec3 Dielectric::local(const Scene& scene, const Ray& r, const Hit& h) const
{
    return h.front_face ? scene.local_terms(r, h).specular : glm::vec3(0.0f);
}

/// Aproximación de Schlick de la reflectancia de Fresnel
///     F(θ) = F0 + (1 - F0) (1 - cos θ)^5,   F0 = ((1 - η) / (1 + η))^2
///
/// cosine es el coseno del ángulo del lado del medio menos denso: el de
/// incidencia si el rayo entra a un medio más denso (η < 1), el de
/// transmisión si no
static float schlick(float cosine, float eta)
{
    float r0 = (1.0f - eta) / (1.0f + eta);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow(1.0f - cosine, 5.0f);
}

int Dielectric::secondary(const Ray& in, const Hit& h, Secondary out[2]) const
{
    const glm::vec3 d = glm::normalize(in.d());

    // η_i / η_t según el rayo entre o salga del objeto
    const float eta = h.front_face ? (1.0f / ior) : ior;

    // Rayo reflejado: siempre existe
    out[0].ray = Ray(h.p + h.n * EPSILON, glm::reflect(d, h.n));

    // Snell: sin θt = η sin θi, así que cos²θt = 1 - η² (1 - cos²θi).
    // Si da negativo, sin θt > 1: no hay rayo refractado (reflexión interna
    // total) y toda la luz se refleja.
    // Una sola cuenta decide las dos cosas. Si la prueba se hace con sin θt y
    // la dirección con glm::refract (que hace su propia cuenta), cerca del
    // ángulo límite el redondeo puede aprobar una y no la otra: glm::refract
    // devuelve el vector nulo, normalizarlo da NaN y el píxel sale negro
    const float cos_i = glm::min(glm::dot(-d, h.n), 1.0f);
    const float k = 1.0f - eta * eta * (1.0f - cos_i * cos_i);
    if( k < 0.0f ){
        out[0].weight = 1.0f;
        return 1;
    }
    const float cos_t = glm::sqrt(k);

    // Rayo refractado: T = η d + (η cos θi - cos θt) n.
    // El origen se corre hacia el otro lado de la superficie
    const glm::vec3 T = eta * d + (eta * cos_i - cos_t) * h.n;
    out[1].ray = Ray(h.p - h.n * EPSILON, glm::normalize(T));

    // Coseno del lado del medio menos denso (ver schlick). Mirar η en lugar de
    // front_face cubre también los materiales con ior < 1, como la superficie
    // interior de una esfera hueca
    const float F = schlick((eta <= 1.0f) ? cos_i : cos_t, eta);

    out[0].weight = F;
    out[1].weight = 1.0f - F;
    return 2;
}
