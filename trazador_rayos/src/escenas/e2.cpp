// Escena 0: corresponde al taller 1, iluminación local con sombras.
//
// Base para la composición de la imagen "Spheres and Checkerboard" (Whitted,
// 1980). Lo que se construye en el taller 1: un plano infinito con tablero y dos
// esferas difusas con brillo de Phong, iluminadas por una luz puntual que hace
// sombra. En la imagen de Whitted el piso es un rectángulo finito, la esfera
// de adelante es de vidrio y la de atrás, un espejo.

#include <memory>

#include <triangle.h>
#include <checker.h>
#include <mirror.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>

void build_scene_2(Scene& scene, Camera& camera)
{
    /*Cámara: ojo (-2, 2, 7.5), mira a (3, 1.4, 0), arriba (0, 1, 0), fov vertical 45 [°].
Fondo: uniforme (0.02, 0.03, 0.08).
Piso: plano por (0, 0, 0), normal (0, 1, 0). Checker: size=1, kd=(0.7, 0.7, 0.7), ka=(0.07, 0.07,
0.07), kd2=(0.1, 0.1, 0.1), ka2=(0.01, 0.01, 0.01).
Paredes: rectángulos en x = −3 y en x = 3, con y de 0 a 5 y z de -12 a 14. Con a=(x, 0, 14), b=(x, 0,
-12), c=(x, 5, -12), d=(x, 5, 14), los triángulos son a-b-c y a-c-d. Mirror: ka=(0, 0, 0), kd=(0.02,
0.02, 0.02), ks=(0, 0, 0), kr=0.9.
Esfera roja: centro (0.2, 1, 0), radio 1. Diffuse: ka=(0.08, 0.01, 0.01), kd=(0.75, 0.08, 0.06),
ks=(0.5, 0.5, 0.5), shininess=60.
Esfera verde: centro (1.2, 0.6, 2.5), radio 0.6. Diffuse: ka=(0.01, 0.06, 0.02), kd=(0.1, 0.6,
0.2), ks=(0.4, 0.4, 0.4), shininess=40.
Esfera espejo: centro (-1.3, 0.9, -2.5), radio 0.9. Mirror: ka=(0.02, 0.02, 0.02), kd=(0.1, 0.1,
0.1), ks=(1, 1, 1), shininess=150, kr=0.85.
Luz: posición (0, 9, 5), color (4, 4, 4).*/

    camera.look_at(glm::vec3(-2.0f, 2.0f, 7.5f),     // ojo
                   glm::vec3(3.0f, 1.4f, 0.0f),     // hacia dónde mira
                   glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
                   45.0f);                          // fov vertical [grados]

    // Celeste (valores lineales; aproximadamente (25,160,200) después de la gamma)
    scene.set_background(glm::vec3(0.02f, 0.03f, 0.08f));

    // --- Materiales -----------------------------------------------------------

    // Piso: tablero rojo y amarillo. ka también sigue al tablero, para que las
    // casillas se distingan dentro de las sombras
    auto floor = std::make_shared<Checker>();
    floor->size = 1.0f;
    floor->kd   = glm::vec3(0.7f);   // rojo
    floor->ka   = glm::vec3(0.07f);
    floor->kd2  = glm::vec3(0.1f);    // amarillo
    floor->ka2  = glm::vec3(0.01f);

    auto wall = std::make_shared<Mirror>();
    wall->kd   = glm::vec3(0.02f);   // rojo
    wall->ka   = glm::vec3(0.0f);
    wall->ks  = glm::vec3(0.0f);    // amarillo
    wall->kr  = 0.9f;

    // --- Geometría ------------------------------------------------------------

    glm::vec3 a(-3.0,0.0,14.0);
    glm::vec3 b(-3.0f, 0.0f, -12.0f);
    glm::vec3 c(-3.0f, 5.0f, -12.0f);
    glm::vec3 d(-3.0f, 5.0f,  14.0f);


    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
    scene.add_object(std::make_shared<Triangle>(a,b,c,wall));
    scene.add_object(std::make_shared<Triangle>(a,c,d,wall));

    // --- Luces ----------------------------------------------------------------

    // Alta y un poco por delante de las esferas, para que los brillos queden
    // hacia el observador. Con atenuación 1 / (1 + 0.12 d + 0.02 d^2)
    const glm::vec3 att(1.0f, 0.12f, 0.02f);
    scene.add_light(Light(glm::vec3(0.0f, 9.0f, 5.0f), glm::vec3(4.0f),
                          att));
}
