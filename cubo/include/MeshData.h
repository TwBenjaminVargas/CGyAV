#pragma once

#include <vector>

/**
 * @brief Tupla de atributos para un vértice en formato intercalado (AoS).
 */
struct Vertex {
    float px, py, pz; // Posición (X, Y, Z)
    float r, g, b;    // Color (R, G, B)
};

/**
 * @brief Datos de la malla en el lado de la CPU.
 */
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; // Vacío si la malla no es indexada
};