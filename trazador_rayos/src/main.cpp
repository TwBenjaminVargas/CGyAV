// -----------------------------------------------------------------------------
// main.cpp: trazador de rayos. CGyAV 2026, IUA.
//
// Uso:  ./bin/rrt [escena] [opciones]
//
//   escena        número de la escena (por defecto, la primera de la tabla)
//   -o archivo    imagen de salida en PNG (por defecto, escena-NN.png)
//   -r W H        resolución (por defecto 800 600)
//   -ss N         supermuestreo de N x N rayos por píxel (por defecto 1)
//   -n            vista de control: la normal como color, sin iluminación
//   -l            lista las escenas disponibles
// -----------------------------------------------------------------------------

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <camera.h>
#include <escenas.h>
#include <image.h>
#include <scene.h>

static void list_scenes(void)
{
    for( int k = 0; k < kSceneCount; ++k ){
        std::printf("  %2d  %s\n", kScenes[k].id, kScenes[k].name);
    }
}

int main(int argc, char** argv)
{
    int scene_id = kScenes[0].id;
    std::string out_path;
    int W = 800, H = 600;
    int ss = 1;
    bool normals = false;

    // Procesar los argumentos de la línea de comando
    for( int a = 1; a < argc; ++a ){
        if( !std::strcmp(argv[a], "-o") && a + 1 < argc ){
            out_path = argv[++a];
        } else if( !std::strcmp(argv[a], "-r") && a + 2 < argc ){
            W = std::atoi(argv[++a]);
            H = std::atoi(argv[++a]);
        } else if( !std::strcmp(argv[a], "-ss") && a + 1 < argc ){
            ss = std::atoi(argv[++a]);
        } else if( !std::strcmp(argv[a], "-n") ){
            normals = true;
        } else if( !std::strcmp(argv[a], "-l") ){
            list_scenes();
            return 0;
        } else {
            scene_id = std::atoi(argv[a]);
        }
    }
    if( W <= 0 || H <= 0 || ss <= 0 ){
        std::cerr << "Resolución o supermuestreo inválidos\n";
        return 1;
    }

    // Leer los datos de la escena para poder construirla
    const SceneEntry* scene_entry = nullptr;
    for( int k = 0; k < kSceneCount; ++k ){
        if( kScenes[k].id == scene_id ) scene_entry = &kScenes[k];
    }
    if( !scene_entry ){
        std::cerr << "No existe la escena " << scene_id << ". Disponibles:\n";
        list_scenes();
        return 1;
    }
    if( out_path.empty() ){
        char name[32];
        std::snprintf(name, sizeof(name), normals ? "escena-%02d-normales.png" : "escena-%02d.png", scene_id);
        out_path = name;
    }

    Scene scene;
    Camera camera;
    scene_entry->build(scene, camera);

    std::clog << "Escena " << scene_id << ": " << scene_entry->name << "\n"
              << W << " x " << H << ", " << ss << " x " << ss << " rayos por píxel\n";

    // Supermuestreo: grilla de ss x ss rayos dentro de cada píxel,
    // promediados con el mismo peso
    const float weight = 1.0f / float(ss * ss);

    Image img(W, H);
    const auto t0 = std::chrono::steady_clock::now();

    // Las filas se reparten entre los núcleos; dynamic: cada hilo toma la
    // siguiente fila libre cuando termina una (no todas tienen el mismo
    // costo computacional ej. rayos directo al fondo vs intersección con
    // dielectro)
#pragma omp parallel for schedule(dynamic, 1)
    for( int j = 0; j < H; ++j ){
        for( int i = 0; i < W; ++i ){
            glm::vec3 col(0.0f);

            // Supermuestreo
            for( int sy = 0; sy < ss; ++sy ){
                for( int sx = 0; sx < ss; ++sx ){
                    const glm::vec2 offset((sx - 0.5f * (ss - 1)) / float(ss),
                                           (sy - 0.5f * (ss - 1)) / float(ss));

                    // Rayo de la muestra actual
                    const Ray r = camera.primary_ray(i, j, W, H, offset);

                    // Color de la muestra actual
                    const glm::vec3 c = normals ? scene.normal_color(r) : scene.trace(r);

                    // Cada muestra se recorta antes de promediar: en el modelo
                    // de iluminación de Phong se pueden acumular valores mayores
                    // que 1.0
                    col += glm::clamp(c, glm::vec3(0.0f), glm::vec3(1.0f)) * weight;
                }
            }
            img.at(i, j) = col;
        }
    }

    // Medir cuanto tiempo demoró en generarse la imagen
    const auto t1 = std::chrono::steady_clock::now();
    std::clog << "Render: "
              << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";

    // Las normales son datos, no luz: se escriben sin corrección gamma
    if( !img.write(out_path, normals ? 1.0f : 2.2f) ){
        std::cerr << "No se pudo escribir " << out_path << "\n";
        return 1;
    }
    std::clog << "Imagen: " << out_path << "\n";
    return 0;
}
