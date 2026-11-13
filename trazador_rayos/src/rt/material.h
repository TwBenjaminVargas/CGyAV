#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include <hit.h>
#include <ray.h>

class Scene;

/// Iluminación local de Phong separada en sus tres términos, para que cada
/// material pueda hacer uso del que necesite (el vidrio, por ejemplo, sólo usa
/// el brillo)
struct LocalTerms {
    glm::vec3 ambient  = glm::vec3(0.0f);
    glm::vec3 diffuse  = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);

    glm::vec3 total(void) const { return ambient + diffuse + specular; }
};

/// Rayo secundario que genera un material, con el peso con el que su color
/// entra en el del punto: kr en el espejo, F y 1 - F en el vidrio
struct Secondary {
    Ray ray;
    float weight = 0.0f;
};

/// Clase base de los materiales: los coeficientes del modelo de iluminación de
/// Phong y la forma en que el material usa la iluminación local. Las
/// implementaciones por defecto son las del material difuso y opaco; un
/// material nuevo deriva de esta clase y sobrescribe lo que cambia.
///
/// Modelo de Whitted:  I = I_local + suma de (peso_i * I(rayo_i))
///
/// Cada material responde tres preguntas:
///   local()               qué parte de Phong usa (se la pide a la escena)
///   secondary()           qué rayos secundarios genera y con qué peso
///   shadow_transmission() cuánta luz deja pasar a un rayo de sombra
class Material {
public:
    glm::vec3 ka = glm::vec3(0.02f);    ///< ambiente
    glm::vec3 kd = glm::vec3(0.8f);     ///< difuso
    glm::vec3 ks = glm::vec3(0.0f);     ///< especular (brillo de Phong)
    float shininess = 32.0f;            ///< exponente de Phong

    virtual ~Material() = default;

    /// Coeficientes en el punto p: una textura los hace variar con p
    virtual glm::vec3 ka_at(const glm::vec3& /*p*/) const { return ka; }
    virtual glm::vec3 kd_at(const glm::vec3& /*p*/) const { return kd; }

    /// Iluminación local en un punto. Le pide a la escena los términos de
    /// Phong (que trazan los rayos de sombra) sólo si los va a usar
    virtual glm::vec3 local(const Scene& scene, const Ray& r, const Hit& h) const;

    /// Escribe hasta 2 rayos en out y devuelve cuántos escribió
    virtual int secondary(const Ray& /*in*/, const Hit& /*h*/, Secondary /*out*/[2]) const {
        return 0;
    }

    /// 0: opaco (sombra plena); 1: no hace sombra
    virtual float shadow_transmission(void) const { return 0.0f; }
};

/// Difuso: Phong completo, sin rayos secundarios. Es el comportamiento de la
/// clase base; la clase existe para que la escena diga qué material usa
class Diffuse : public Material {};

#endif // MATERIAL_H
