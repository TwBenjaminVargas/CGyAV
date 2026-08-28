#include <cstdlib>                               // Incluye macros estándar de C++ como EXIT_SUCCESS y EXIT_FAILURE
#include <iostream>                              // Incluye utilidades de entrada/salida para consola
#include <string>                                // Incluye el tipo de dato std::string

#include <glad/gl.h>                             // Incluye definiciones y funciones de OpenGL
#include <GLFW/glfw3.h>                          // Incluye las funciones para ventana e inputs

// -----------------------------------------------------------------------------
// SHADERS EN GLSL
// -----------------------------------------------------------------------------
// =============================================================================
// VERTEX SHADER
// =============================================================================
const char* vertexShaderSource = R"(
#version 450 core                                // Define la versión de GLSL (4.5) y que usa el perfil "Core"
layout (location = 0) in vec3 aPos;              // Declara la variable de entrada 'aPos' (vector de 3 floats) en la ubicación de atributo 0

void main() {                                    // Función principal del Vertex Shader (se ejecuta una vez por vértice)
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); // Asigna la posición final en la variable del sistema. Agrega w=1.0 para coordenadas homogéneas
}                                                // Fin de la función main
)";

// =============================================================================
// FRAGMENT SHADER
// =============================================================================
const char* fragmentShaderSource = R"(
#version 450 core                                // Define la versión de GLSL (4.5) y el perfil "Core"
out vec4 FragColor;                              // Declara la variable de salida 'FragColor' (vector de 4 floats: R, G, B, A)

void main() {                                    // Función principal del Fragment Shader (se ejecuta una vez por cada píxel a dibujar)
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);   // Asigna el color RGBA definitivo al píxel (Naranja con opacidad 100%)
}                                                // Fin de la función main
)";

// Callbacks básicos
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);             // Redimensiona el viewport al cambiar la ventana
}

void processInput(GLFWwindow *window) {          
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);  // Cierra si presionas ESC
    }
}

int main() {                                     
    if (!glfwInit()) return EXIT_FAILURE;        // Inicializa GLFW
    
    // Solicitamos contexto OpenGL 4.5 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL 4.5 - Triangulo DSA", nullptr, nullptr);
    if (!window) {                               
        glfwTerminate();                        
        return EXIT_FAILURE;                    
    }
    glfwMakeContextCurrent(window);             

    if (!gladLoadGL(glfwGetProcAddress)) {       // Carga punteros de OpenGL con GLAD
        glfwDestroyWindow(window);              
        glfwTerminate();                        
        return EXIT_FAILURE;                    
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------------------------------------------------------
    // 1. COMPILACIÓN Y LINKEO DE SHADERS
    // -------------------------------------------------------------------------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   
    glCompileShader(vertexShader);                               

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);   
    glCompileShader(fragmentShader);                             

    GLuint shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertexShader);    
    glAttachShader(shaderProgram, fragmentShader);  
    glLinkProgram(shaderProgram);                   

    glDeleteShader(vertexShader);                   
    glDeleteShader(fragmentShader);                 

    // -------------------------------------------------------------------------
    // 2. DEFINICIÓN Y CONFIGURACIÓN CON DSA (Direct State Access)
    // -------------------------------------------------------------------------
    float vertices[] = {                            
        -0.5f, -0.5f, 0.0f,                      // Esquina inferior izquierda
         0.5f, -0.5f, 0.0f,                      // Esquina inferior derecha
         0.0f,  0.5f, 0.0f                       // Esquina superior centro
    };

    GLuint vao = 0;
    GLuint vbo = 0;

    // Crea los objetos VAO y VBO directamente en memoria de la GPU
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    // Inyecta los datos al VBO directamente por su ID
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Activa la ubicación 'location = 0' en el VAO
    glEnableVertexArrayAttrib(vao, 0);

    // Configura el formato del atributo 0 (3 floats de tipo GL_FLOAT)
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    // Mapea el atributo 0 al punto de enlace 0 (binding index 0)
    glVertexArrayAttribBinding(vao, 0, 0);

    // Conecta el VBO al punto de enlace 0 del VAO indicando el salto (stride = 3 floats)
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));

    // -------------------------------------------------------------------------
    // 3. BUCLE PRINCIPAL
    // -------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {        
        processInput(window);                       

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);       
        glClear(GL_COLOR_BUFFER_BIT);               

        glUseProgram(shaderProgram);                
        
        // Enlazamos el VAO únicamente para dibujarlo
        glBindVertexArray(vao);                     
        glDrawArrays(GL_TRIANGLES, 0, 3);           

        glfwSwapBuffers(window);                   
        glfwPollEvents();                          
    }

    // -------------------------------------------------------------------------
    // 4. LIMPIEZA
    // -------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);                  
    glDeleteBuffers(1, &vbo);                       
    glDeleteProgram(shaderProgram);                 

    glfwDestroyWindow(window);                     
    glfwTerminate();                               
    return EXIT_SUCCESS;                           
}