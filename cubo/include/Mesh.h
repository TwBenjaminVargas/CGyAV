#pragma once

#include "MeshData.h"

/**
 * @brief Gestiona los recursos de GPU (VAO, VBO, EBO) de forma segura.
 */
class Mesh {
public:
    Mesh() = default;
    ~Mesh(); // Libera los recursos de la GPU (VAO, VBO, EBO)

    // PROHIBICIÓN DE COPIA

    // Copiar rompe el dueño único y provocaría doble liberación en la GPU
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // SEMÁNTICA DE MOVIMIENTO 

    // Traspasa la propiedad de los recursos de OpenGL de un objeto a otro
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    /**
     * @brief Sube los datos a la GPU (VBO/EBO) y arma el VAO usando estilo DSA.
     */
    void load(const MeshData& data);

    /**
     * @brief Elimina los buffers de la GPU y restablece el estado del objeto a 0.
     */
    void clear(void);

    // --- GETTERS ---
    unsigned int vao(void) const { return vao_; }
    int count(void) const { return count_; } // Cantidad de ÍNDICES (o VÉRTICES si no hay índices)

private:
    unsigned int vao_{0U};
    unsigned int vbo_{0U};
    unsigned int ebo_{0U};
    int count_{0};
};