// Escena 0: corresponde al taller 1, iluminación local con sombras.
//
// Base para la composición de la imagen "Spheres and Checkerboard" (Whitted,
// 1980). Lo que se construye en el taller 1: un plano infinito con tablero y dos
// esferas difusas con brillo de Phong, iluminadas por una luz puntual que hace
// sombra. En la imagen de Whitted el piso es un rectángulo finito, la esfera
// de adelante es de vidrio y la de atrás, un espejo.

#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>

void build_scene_00(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(0.0f, 2.0f, 8.0f),     // ojo
                   glm::vec3(0.3f, 1.6f, 0.0f),     // hacia dónde mira
                   glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
                   45.0f);                          // fov vertical [grados]

    // Celeste (valores lineales; aproximadamente (25,160,200) después de la gamma)
    scene.set_background(glm::vec3(0.01f, 0.36f, 0.58f));

    // --- Materiales -----------------------------------------------------------

    // Piso: tablero rojo y amarillo. ka también sigue al tablero, para que las
    // casillas se distingan dentro de las sombras
    auto floor = std::make_shared<Checker>();
    floor->size = 0.7f;
    floor->kd   = glm::vec3(0.75f, 0.02f, 0.02f);   // rojo
    floor->ka   = glm::vec3(0.20f, 0.005f, 0.005f);
    floor->kd2  = glm::vec3(0.80f, 0.75f, 0.0f);    // amarillo
    floor->ka2  = glm::vec3(0.20f, 0.19f, 0.0f);
    floor->ks   = glm::vec3(0.0f);
    floor->shininess = 1.0f;

    // Esfera de adelante (en la imagen de Whitted es la de vidrio): blanca
    auto white = std::make_shared<Diffuse>();
    white->ka = glm::vec3(0.05f);
    white->kd = glm::vec3(0.8f);
    white->ks = glm::vec3(0.6f);
    white->shininess = 60.0f;

    // Esfera de atrás (en la imagen de Whitted es el espejo): azul
    auto blue = std::make_shared<Diffuse>();
    blue->ka = glm::vec3(0.01f, 0.02f, 0.05f);
    blue->kd = glm::vec3(0.10f, 0.20f, 0.70f);
    blue->ks = glm::vec3(0.6f);
    blue->shininess = 60.0f;

    // --- Geometría ------------------------------------------------------------

    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-0.2f, 2.2f, 3.0f), 1.0f, white));
    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.25f, 1.2f, 1.3f), 1.0f, blue));

    // --- Luces ----------------------------------------------------------------

    // Alta y un poco por delante de las esferas, para que los brillos queden
    // hacia el observador. Con atenuación 1 / (1 + 0.12 d + 0.02 d^2)
    scene.add_light(Light(glm::vec3(0.0f, 12.0f, 8.0f), glm::vec3(5.0f),
                          glm::vec3(1.0f, 0.12f, 0.02f)));
}
