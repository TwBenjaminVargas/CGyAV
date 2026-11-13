# Trazador de rayos: esqueleto del taller de iluminación local

Estructura para construir un generador de imagenes usando un trazador de rayos
(*ray tracer*). EL programa lanza un rayo por cada píxel para determinar el color
del mismo y, como la escena está vacía, todos los rayos terminan en el fondo celeste. El programa compila y escribe esta imagen base. Durante el taller se
amplía el trazador en seis pasos agregando distintas funcionalidades: objetos, 
iluminación y texturas.

## Requisitos y compilación

Se usa el mismo entorno que en el resto de la materia: `g++`, `make` y GLM (en
Windows, la terminal MinGW64 de MSYS2). No hace falta GLFW ni OpenGL, porque
el programa no abre ninguna ventana. La salida de las imágenes se hace en 
formato PNG por lo que se incluye `stb_image_write` en la carpeta `third_party/`
para escribir las mismas.

```sh
make                 # compila bin/rrt
./bin/rrt            # renderiza la escena 0 y escribe escena-00.png
./bin/rrt -n         # vista de normales (paso 3): escena-00-normales.png
./bin/rrt -l         # lista las escenas disponibles
```

**Otras opciones:** `-o archivo.png` para elegir el nombre de la salida, `-r W H`
para cambiar la resolución (por defecto 800 x 600 [píxeles]) y `-ss N` para usar
supermuestreo, es decir, cada pixel de la imagen se genera a partir de N x N
rayos dentro del pixel promediados (por defecto 1: sin sobre muestreo). Con
`-ss 3 -r 1280 720` se obtiene una imagen de mayor calidad pero que demora mucho
más tiempo en generarse, sirve para obtener la imagen final luego de haber hecho
los ajustes de la escena o el agregado de una nueva funcionalidad.

**Optimización:** paralelización de la generación de la imagen. El trazador 
reparte las filas de la imagen entre los núcleos del procesador usando OpenMP
(directiva de preprocesador `#pragma omp parallel for schedule(dynamic, 1)` en
`main.cpp` y opción de compilación `-fopenmp` en el `Makefile`).

## Organización del código

Se usa una organización modular que separa la geometría, los materiales y la
escena, de modo que agregar un objeto o un material nuevo no obligue a tocar el
resto.

| Carpeta / archivo | Contenido |
|---|---|
| `src/main.cpp` | App principal. Opciones, recorrido de los píxeles, supermuestreo y escritura de la imagen |
| `src/rt/ray.h`, `camera.h` | Modelo del rayo P(t) = o + t d y de la cámara que genera un rayo por píxel |
| `src/rt/object.h`, `plane.h`, `sphere.h` | Clase base para la geometría y dos especializaciones: planos y esferas |
| `src/rt/hit.h` | Datos en la intersección de un rayo con una geometría: t, punto, normal y material |
| `src/rt/material.h`, `checker.h` | Clase base de los materiales (modelo de Phong) y una especialización para una textura tipo tablero |
| `src/rt/light.h` | Modelo de luz puntual con atenuación opcional con la distancia |
| `src/rt/scene.h`, `scene.cpp` | La escena: objetos, luces, fondo y el procedimiento de trazado de rayos |
| `src/rt/image.h`, `image.cpp` | Imagen con salidad a archivos PNG y corrección gamma |
| `src/escenas/` | Directorio del módulo de soporte para poder generar distintas escenas por código: una función por escena y tabla que lleva el conteo de escenas disponibles |

## Actividades para el taller: seis pasos

Cada actividad a desarrollar está marcada en el código con un comentario 
`TODO (paso N)` con una breve explicación qué indica que hay que modificar /
agregar para completar la actividad.

| Paso | Dónde | Cuál es el resultado |
|---|---|---|
| 1. Intersección rayo-plano | `Plane::intersect` (`plane.h`) | La mitad inferior de la imagen pasa a ser el piso, de un color sólido (ej. rojo) |
| 2. Intersección rayo-esfera | `Sphere::intersect` (`sphere.h`) | Aparecen dos discos planos de distinto color, ej. uno blanco y otro azul, sin volumen |
| 3. Normales como color | `Scene::normal_color` (`scene.cpp`) | Ejecutando con la opción `-n` se observa: el piso cambia a verde y las esferas con un degradé de colores que cambia con la orientación de la superficie |
| 4. Modelo de iluminación de Phong | `Scene::local_terms` (`scene.cpp`) | Las esferas toman volumen y brillo y el piso se oscurece hacia el fondo |
| 5. Sombras | `Scene::light_transmission` (`scene.cpp`) | Las esferas proyectan sombra sobre el piso, y la que está en primer plano (ej. blanca), sobre la otra (ej. azul) |
| 6. Textura de tablero | `Checker::odd` (`checker.h`) | El piso pasa a ser un tablero rojo y amarillo |

Hasta el paso 4, para el color de cada objeto se toma el coeficiente difuso (lo
que genera un color sólido sobre el objeto), así se pueden verificar que las 
intersecciones sean correctas antes de escribir la iluminación.

## Agregar una escena

Cada escena es una función `build_scene_NN(Scene&, Camera&)` que arma la
geometría, los materiales, las luces y la cámara y devuelve por referencia todo
esto en `Scene&` y `Camera&`. Para agregar una escena nueva hay que:

1. Escribirla en un archivo nuevo en `src/escenas/`, tomando como modelo
   `e00-taller1.cpp`.
2. Declararla en `escenas.h` y sumarla a la tabla de `escenas.cpp`.
3. Compilar y renderizarla con `./bin/rrt NN`.

El `Makefile` compila solo cualquier `.cpp` nuevo de `src/rt/` o de
`src/escenas/` por lo que no hay que hacer nada más.
