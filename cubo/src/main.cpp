#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Mesh.h"
#include "Primitives.h"
#include "ResourceManager.hpp"

// Función con verificación de compilación y enlace
GLuint compileShaderPipeline(const ShaderSource& source) {
    GLint success = 0;
    char infoLog[512];

    // 1. Vertex Shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    const char* vsCode = source.vs.c_str();
    glShaderSource(vert, 1, &vsCode, nullptr);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 512, nullptr, infoLog);
        glDeleteShader(vert);
        throw std::runtime_error(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog);
    }

    // 2. Fragment Shader
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsCode = source.fs.c_str();
    glShaderSource(frag, 1, &fsCode, nullptr);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 512, nullptr, infoLog);
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw std::runtime_error(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") + infoLog);
    }

    // 3. Linkeo del Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        glDeleteShader(vert);
        glDeleteShader(frag);
        glDeleteProgram(program);
        throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cubo 3 Colores - Flight Sim", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram = 0;

    // --- CARGA DE SHADERS CON RESOURCE MANAGER ---
    try {
        // Se corrige la ruta raíz a "assets" para no duplicar carpeta con "shaders/..."
        ResourceManager resourceManager("assets");
        int shaderKey = -1;

        const ShaderSource& source = resourceManager.load_shader_source(
            shaderKey, 
            "shaders/test.vert", 
            "shaders/test.frag"
        );

        shaderProgram = compileShaderPipeline(source);
        std::cout << "[OK] Shaders compilados y vinculados correctamente. Key: " << shaderKey << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[ERROR CRITICO]: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // --- CARGA DE LA MALLA ---
    Mesh cubeMesh;
    try {
        MeshData cubeData = primitives::cube(1.0f, 1.0f, 1.0f);
        cubeMesh.load(cubeData);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error al cargar la malla: " << e.what() << std::endl;
        return -1;
    }

    // --- BUCLE DE RENDERIZADO ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(cubeMesh.vao());

        GLint eboBinding = 0;
        glGetVertexArrayiv(cubeMesh.vao(), GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBinding);

        if (eboBinding != 0) {
            glDrawElements(GL_TRIANGLES, cubeMesh.count(), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, cubeMesh.count());
        }

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}