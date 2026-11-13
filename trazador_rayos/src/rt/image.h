#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

/// Imagen en color lineal, tamaño W x H, fila 0 arriba
struct Image {
    int W = 0, H = 0;
    std::vector<glm::vec3> pixels;

    Image(int w, int h) : W(w), H(h), pixels(size_t(w) * size_t(h), glm::vec3(0.0f)) {}

    glm::vec3& at(int i, int j) { return pixels[size_t(j) * size_t(W) + size_t(i)]; }

    /// Escribe la imagen en PNG. Antes de pasar a 8 bits recorta cada componente
    /// a [0,1] y aplica la corrección gamma (Práctico 09): el trazador calcula
    /// en espacio lineal y el monitor espera valores codificados. Con gamma = 1
    /// deja los valores como están (sirve para imágenes de datos, como las
    /// normales)
    bool write(const std::string& path, float gamma = 2.2f) const;
};

#endif // IMAGE_H
