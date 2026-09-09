#include "Primitives.h"

namespace primitives {

MeshData cube(float scale_x, float scale_y, float scale_z)
{
    float x = scale_x * 0.5f;
    float y = scale_y * 0.5f;
    float z = scale_z * 0.5f;

    MeshData data;

    // Colores por cara
    float r_frente  = 0.8f, g_frente  = 0.2f, b_frente  = 0.2f; // rojo
    float r_atras   = 0.8f, g_atras   = 0.5f, b_atras   = 0.2f; // naranja
    float r_derecha = 0.9f, g_derecha = 0.7f, b_derecha = 0.1f; // amarillo
    float r_izq     = 0.2f, g_izq     = 0.5f, b_izq     = 0.8f; // azul
    float r_arriba  = 0.8f, g_arriba  = 0.8f, b_arriba  = 0.8f; // gris
    float r_abajo   = 0.2f, g_abajo   = 0.7f, b_abajo   = 0.3f; // verde

    data.vertices = {
        // CARA FRENTE (z = +z), normal apunta hacia +Z
        {-x, -y,  z,  r_frente,  g_frente,  b_frente},
        { x, -y,  z,  r_frente,  g_frente,  b_frente},
        { x,  y,  z,  r_frente,  g_frente,  b_frente},
        {-x,  y,  z,  r_frente,  g_frente,  b_frente},

        // CARA ATRÁS (z = -z), normal apunta hacia -Z
        { x, -y, -z,  r_atras,   g_atras,   b_atras},
        {-x, -y, -z,  r_atras,   g_atras,   b_atras},
        {-x,  y, -z,  r_atras,   g_atras,   b_atras},
        { x,  y, -z,  r_atras,   g_atras,   b_atras},

        // CARA DERECHA (x = +x), normal apunta hacia +X
        { x, -y,  z,  r_derecha, g_derecha, b_derecha},
        { x, -y, -z,  r_derecha, g_derecha, b_derecha},
        { x,  y, -z,  r_derecha, g_derecha, b_derecha},
        { x,  y,  z,  r_derecha, g_derecha, b_derecha},

        // CARA IZQUIERDA (x = -x), normal apunta hacia -X
        {-x, -y, -z,  r_izq,     g_izq,     b_izq},
        {-x, -y,  z,  r_izq,     g_izq,     b_izq},
        {-x,  y,  z,  r_izq,     g_izq,     b_izq},
        {-x,  y, -z,  r_izq,     g_izq,     b_izq},

        // CARA ARRIBA (y = +y), normal apunta hacia +Y
        {-x,  y,  z,  r_arriba,  g_arriba,  b_arriba},
        { x,  y,  z,  r_arriba,  g_arriba,  b_arriba},
        { x,  y, -z,  r_arriba,  g_arriba,  b_arriba},
        {-x,  y, -z,  r_arriba,  g_arriba,  b_arriba},

        // CARA ABAJO (y = -y), normal apunta hacia -Y
        {-x, -y, -z,  r_abajo,   g_abajo,   b_abajo},
        { x, -y, -z,  r_abajo,   g_abajo,   b_abajo},
        { x, -y,  z,  r_abajo,   g_abajo,   b_abajo},
        {-x, -y,  z,  r_abajo,   g_abajo,   b_abajo},
    };

    data.indices = {
         0,  1,  2,   0,  2,  3,  // frente
         4,  5,  6,   4,  6,  7,  // atrás
         8,  9, 10,   8, 10, 11,  // derecha
        12, 13, 14,  12, 14, 15,  // izquierda
        16, 17, 18,  16, 18, 19,  // arriba
        20, 21, 22,  20, 22, 23,  // abajo
    };

    return data;
}

} // namespace primitives