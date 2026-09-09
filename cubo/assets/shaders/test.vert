#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

// Matriz asignada por columnas según la especificación de GLSL:
// Columna 0: vec3( 0.3686, -0.1454, -0.3119)
// Columna 1: vec3( 0.0000,  0.5438, -0.2536)
// Columna 2: vec3(-0.2581, -0.2077, -0.4454)
const mat3 kRotacionFija = mat3(
    vec3( 0.3686, -0.1454, -0.3119),
    vec3( 0.0000,  0.5438, -0.2536),
    vec3(-0.2581, -0.2077, -0.4454)
);

void main() {
    // Aplicamos la matriz de transformación a los vértices
    vec3 rotatedPos = kRotacionFija * aPos;

    gl_Position = vec4(rotatedPos, 1.0);
    vertexColor = aColor;
}