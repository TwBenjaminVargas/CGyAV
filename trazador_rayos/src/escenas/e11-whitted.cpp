// Escena 11: "Spheres and Checkerboard" (Whitted, 1980)
//
// Una esfera de vidrio hueca y grande flota adelante, superpuesta a una esfera
// espejo más chica ubicada atrás, sobre un tablero finito rojo y amarillo y con
// cielo celeste.

#include <memory>

#include <checker.h>
#include <dielectric.h>
#include <escenas.h>
#include <mirror.h>
#include <sphere.h>
#include <triangle.h>

void build_scene_11(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(0.0f, 2.0f, 8.0f),     // ojo
                   glm::vec3(0.3f, 1.6f, 0.0f),     // hacia dónde mira
                   glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
                   45.0f);                          // fov vertical

    // Celeste (valores lineales; ≈ (25,160,200) después de la gamma)
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

    // Esfera espejo (la más chica, atrás)
    auto metal = std::make_shared<Mirror>();
    metal->ka = glm::vec3(0.03f);
    metal->kd = glm::vec3(0.15f);
    metal->ks = glm::vec3(1.0f);
    metal->shininess = 100.0f;
    metal->kr = 0.85f;

    // Esfera de vidrio hueca (la más grande, adelante): dos esferas concéntricas.
    // Whitted confirmó que la esfera es hueca; la razón de radios 0.96 es la del
    // ejemplo optixWhitted de NVIDIA
    auto glass_out = std::make_shared<Dielectric>();   // superficie exterior: aire -> vidrio
    glass_out->ior = 1.5f;
    glass_out->transparency = 0.8f;

    // Superficie interior: lo de "adentro" de esta esfera es aire rodeado de
    // vidrio, así que su índice relativo es 1/1.5. Sin brillo de Phong: la luz
    // le llega refractada por la cáscara, pero el rayo de sombra va derecho
    auto glass_in = std::make_shared<Dielectric>(*glass_out);
    glass_in->ior = 1.0f / 1.5f;
    glass_in->ks  = glm::vec3(0.0f);

    // --- Geometría ------------------------------------------------------------

    // Piso: rectángulo finito (dos triángulos) en y = 0, x en [-3,20], z en [-14,10]
    const glm::vec3 v00(20.0f, 0.0f,  10.0f);
    const glm::vec3 v01(20.0f, 0.0f, -14.0f);
    const glm::vec3 v02(-3.0f, 0.0f, -14.0f);
    const glm::vec3 v03(-3.0f, 0.0f,  10.0f);
    scene.add_object(std::make_shared<Triangle>(v00, v01, v02, floor));
    scene.add_object(std::make_shared<Triangle>(v00, v02, v03, floor));

    const glm::vec3 glass_center(-0.2f, 2.2f, 3.0f);
    constexpr float kGlassRadius = 1.0f;
    constexpr float kShellRatio  = 0.96f;           // radio interior / exterior
    scene.add_object(std::make_shared<Sphere>(glass_center, kGlassRadius, glass_out));
    scene.add_object(std::make_shared<Sphere>(glass_center, kGlassRadius * kShellRatio, glass_in));

    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.25f, 1.2f, 1.3f), 1.0f, metal));

    // --- Luces ----------------------------------------------------------------

    // Alta y un poco por delante, para que los brillos queden hacia el observador.
    // Con atenuación: a 10-12 unidades de distancia multiplica por ≈ 0,2
    scene.add_light(Light(glm::vec3(0.0f, 12.0f, 8.0f), glm::vec3(5.0f),
                          glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.set_max_depth(8U);
}
