#include <escenas.h>

const SceneEntry kScenes[] = {
    {  0, "Taller 1: plano con tablero, dos esferas difusas, luz puntual", build_scene_00 },
    { 11, "Spheres and Checkerboard (Whitted, 1980)", build_scene_11 },
    { 1, "Ejercicio 1", build_scene_1 },
    { 2, "Ejercicio 2: uso de triangulos y espejos", build_scene_2 }
};

const int kSceneCount = int(sizeof(kScenes) / sizeof(kScenes[0]));
