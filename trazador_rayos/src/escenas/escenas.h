#ifndef ESCENAS_H
#define ESCENAS_H

#include <camera.h>
#include <scene.h>

/// Cada escena arma la geometría, los materiales, las luces y la cámara
using BuildScene = void (*)(Scene& scene, Camera& camera);

struct SceneEntry {
    int id;
    const char* name;
    BuildScene build;
};

/// Una función por escena, cada una en su propio .cpp de esta carpeta
void build_scene_00(Scene& scene, Camera& camera);
void build_scene_11(Scene& scene, Camera& camera);
void build_scene_1(Scene& scene, Camera& camera);
void build_scene_2(Scene& scene, Camera& camera);

/// Tabla de escenas disponibles (escenas.cpp). Para agregar una: escribir su
/// build_scene_NN en un .cpp nuevo, declararla arriba y sumarla a la tabla
extern const SceneEntry kScenes[];
extern const int kSceneCount;

#endif // ESCENAS_H
