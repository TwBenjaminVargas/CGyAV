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

void build_scene_1(Scene& scene, Camera& camera)
{

/**   Cámara: ojo (0, 6, 7), mira a (0, 0.3, 0), arriba (0, 1, 0), fov vertical 45 [°].
Fondo: uniforme (0.02, 0.02, 0.02).
Piso: plano pasando por (0, 0, 0) con normal (0, 1, 0). Diffuse: ka=(0.02, 0.02, 0.02), kd=(0.8,
0.8, 0.8), ks=(0, 0, 0).
Esfera blanca: centro (0, 1, 0), radio 1.0. Diffuse: ka=(0.02, 0.02, 0.02), kd=(0.8, 0.8, 0.8),
ks=(0.3, 0.3, 0.3), shininess=30.
Luz roja: posición (0, 5, -4), color (2.4, 0, 0).
Luz verde: posición (-3.464, 5, 2), color (0, 2.4, 0).
Luz azul: posición (3.464, 5, 2), color (0, 0, 2.4).*/

    camera.look_at(glm::vec3(0.0f, 6.0f, 7.0f),     // ojo
                   glm::vec3(0.0f, 0.3f, 0.0f),     // hacia dónde mira
                   glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
                   45.0f);                          // fov vertical [grados]


    scene.set_background(glm::vec3(0.02f, 0.02f, 0.02f));

    // --- Materiales -----------------------------------------------------------

    auto floor = std::make_shared<Diffuse>();
    floor->kd   = glm::vec3(0.8f);   // rojo
    floor->ka   = glm::vec3(0.02f);
    floor->ks   = glm::vec3(0.0f);

    // Esfera de adelante (en la imagen de Whitted es la de vidrio): blanca
    auto white = std::make_shared<Diffuse>();
    white->ka = glm::vec3(0.02f);
    white->kd = glm::vec3(0.8f);
    white->ks = glm::vec3(0.3f);
    white->shininess = 30.0f;

    // --- Geometría ------------------------------------------------------------

    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f) // pasa por
    , glm::vec3(0.0f, 1.0f, 0.0f), // normal
     floor));
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f),//centro en
        1.0f, // radio
        white));


    // --- Luces ----------------------------------------------------------------
    const glm::vec3 att(1.0f, 0.12f, 0.02f); // atenuacion
    scene.add_light(Light(glm::vec3(0.0f, 5.0f, -4.0f),// posicion
     glm::vec3(2.4f,0.0f,0.0), // color
     att));


    scene.add_light(Light(glm::vec3(-3.464f, 5.0f, 2.0f),// posicion
     glm::vec3(0.0f, 2.4f, 0.0f), // color
     att));

    scene.add_light(Light(glm::vec3(3.464f, 5.0f, 2.0f),// posicion
     glm::vec3(0.0f, 0.0f, 2.4f), // color
     att));
}
