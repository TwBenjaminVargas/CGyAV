#ifndef HIT_H
#define HIT_H

#include <glm/glm.hpp>

#include <defines.h>
#include <ray.h>

class Material;

/// Información que hay que obtener en cada intersección: el punto, la distancia
/// respecto al origen del rayo, la normal de la superficie y el material del
/// objeto.
struct Hit {
    float t = INF;
    glm::vec3 p = glm::vec3(0.0f);
    glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);  ///< normal del lado del rayo (ver set_face)
    bool front_face = true;                     ///< true: el rayo llega desde afuera
    // Puntero crudo (no dueño): el objeto conserva el shared_ptr. Copiar un
    // shared_ptr en cada intersección actualiza un contador atómico que
    // comparten todos los hilos y frena el render en paralelo
    const Material* mat = nullptr;

    /// Recibe la normal geométrica (hacia afuera del objeto) y la orienta hacia
    /// el lado del que viene el rayo. front_face recuerda de qué lado estaba
    void set_face(const Ray& r, const glm::vec3& outward_n) {
        front_face = glm::dot(r.d(), outward_n) < 0.0f;
        n = front_face ? outward_n : -outward_n;
    }
};

#endif // HIT_H
