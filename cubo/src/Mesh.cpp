#include "Mesh.h"
#include <glad/gl.h>
#include <cstddef> // Para offsetof
#include <utility> // Para std::swap
#include <stdexcept> // Para std::invalid_argument

Mesh::~Mesh() {
    clear();
}

void Mesh::clear(void) {
    // Destruye el VAO si existe
    if (vao_ != 0U) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0U;
    }

    // Destruye el VBO si existe
    if (vbo_ != 0U) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0U;
    }

    // Destruye el EBO si existe
    if (ebo_ != 0U) {
        glDeleteBuffers(1, &ebo_);
        ebo_ = 0U;
    }

    count_ = 0;
}

// =============================================================================
// SEMÁNTICA DE MOVIMIENTO (Move Semantics)
// =============================================================================

Mesh::Mesh(Mesh&& other) noexcept 
    : vao_{other.vao_}, vbo_{other.vbo_}, ebo_{other.ebo_}, count_{other.count_} {
    // Deja el objeto 'other' en estado neutro
    other.vao_ = 0U;
    other.vbo_ = 0U;
    other.ebo_ = 0U;
    other.count_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        // Libera los recursos propios antes de recibir los nuevos
        clear();

        // Traspasa los handles
        vao_ = other.vao_;
        vbo_ = other.vbo_;
        ebo_ = other.ebo_;
        count_ = other.count_;

        // Resetea el objeto origen
        other.vao_ = 0U;
        other.vbo_ = 0U;
        other.ebo_ = 0U;
        other.count_ = 0;
    }
    return *this;
}

// =============================================================================
// CARGA DE DATOS A LA GPU (Direct State Access)
// =============================================================================

void Mesh::load(const MeshData& data) {
    // Limpieza preventiva para evitar fugas si el objeto es reutilizado
    clear();

    if (data.vertices.empty()) {
        throw std::invalid_argument("Mesh::load() - Intentando cargar un MeshData sin vértices.");
    }

    // Crear los identificadores (Handles) de OpenGL con estilo DSA
    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);

    // Reservar e inicializar el VBO en VRAM
    const GLsizeiptr vboSize = static_cast<GLsizeiptr>(data.vertices.size() * sizeof(Vertex));
    glNamedBufferData(vbo_, vboSize, data.vertices.data(), GL_STATIC_DRAW); //GL_STATIC_DRAW: los datos no cambian, se usan muchas veces para dibujar

    // Configurar el formato de los atributos en el VAO
    const GLsizei stride = static_cast<GLsizei>(sizeof(Vertex));

    // --- Atributo 0: Posición (px, py, pz) ---
    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(
        vao_, 
        0,                            // Atributo
        3,                            // Componentes (X, Y, Z)
        GL_FLOAT,                     // Tipo de dato
        GL_FALSE,                     // Normalizado
        static_cast<GLuint>(offsetof(Vertex, px)) // Offset dinámico
    );
    glVertexArrayAttribBinding(vao_, 0, 0); // Enlace al Binding Point 0

    // --- Atributo 1: Color (r, g, b) ---
    glEnableVertexArrayAttrib(vao_, 1);
    glVertexArrayAttribFormat(
        vao_, 
        1,                            // Atributo
        3,                            // Componentes (R, G, B)
        GL_FLOAT,                     // Tipo de dato
        GL_FALSE,                     // Normalizado
        static_cast<GLuint>(offsetof(Vertex, r))  // Offset dinámico
    );
    glVertexArrayAttribBinding(vao_, 1, 0); // Enlace al Binding Point 0

    // Vincular el VBO al Binding Point 0 del VAO
    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, stride);

    // Gestionar el EBO si la malla es indexada
    if (!data.indices.empty()) {
        glCreateBuffers(1, &ebo_);
        const GLsizeiptr eboSize = static_cast<GLsizeiptr>(data.indices.size() * sizeof(unsigned int));
        glNamedBufferData(ebo_, eboSize, data.indices.data(), GL_STATIC_DRAW);

        // Enlazar el EBO directamente al VAO
        glVertexArrayElementBuffer(vao_, ebo_);

        count_ = static_cast<int>(data.indices.size());
    } else {
        count_ = static_cast<int>(data.vertices.size());
    }
}