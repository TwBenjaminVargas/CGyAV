#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

/**
 * @brief Estructura que contiene los códigos fuente de los shaders en formato de texto.
 */
struct ShaderSource {
    std::string vs; // Código del Vertex Shader
    std::string fs; // Código del Fragment Shader
    std::string gs; // Código del Geometry Shader (opcional)
};

/**
 * @brief Identificador del tipo de shader para el proceso de lectura de archivos.
 */
enum class ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

class ResourceManager {
public:
    /**
     * @brief Constructor explícito.
     * @param assets_root Ruta base desde la cual se resuelven las rutas relativas de los archivos.
     */
    explicit ResourceManager(const std::filesystem::path& assets_root);

    /**
     * @brief Destructor de la clase. Libera los recursos de memoria.
     */
    ~ResourceManager()=default;

    /**
     * @brief Carga los archivos de shaders desde disco y los inserta en el mapa.
     * 
     * Esta función ignora por completo cualquier valor previo almacenado en la variable apuntada por key.
     * Genera de forma automática una nueva clave autoincremental, escribe dicho valor en la posición
     * de memoria apuntada por key y asigna el ShaderSource al mapa con esa clave.
     * 
     * @param key Referencia a la variable entera del cliente donde se escribirá la clave asignada.
     * @param vs_file Ruta relativa al archivo del Vertex Shader.
     * @param fs_file Ruta relativa al archivo del Fragment Shader.
     * @param gs_file Ruta relativa al archivo del Geometry Shader (opcional).
     * @return const ShaderSource& Referencia al recurso cargado en el mapa.
     * @throws std::invalid_argument Si el puntero key es nullptr.
     * @throws std::runtime_error Si ocurre un error al leer cualquiera de los archivos.
     */
    const ShaderSource& load_shader_source(int& key,
                                           const std::string& vs_file,
                                           const std::string& fs_file,
                                           const std::string& gs_file = "");

    /**
     * @brief Recupera la referencia al ShaderSource asociado a un identificador.
     * @param key Clave entera del shader a buscar.
     * @return const ShaderSource& Referencia de solo lectura al recurso.
     * @throws std::out_of_range Si la clave no existe dentro del mapa.
     */
    const ShaderSource& get_shader_source(int key) const;

    /**
     * @brief Limpia la memoria del mapa y reinicia el contador de claves autoincrementales a 0.
     */
    void clear();

private:
    std::filesystem::path assets_root_;
    std::unordered_map<int, ShaderSource> shaders_sources_;
    int next_key_{0}; // Contador autoincremental de claves

    /**
     * @brief Lee un archivo de texto desde el sistema de archivos.
     * @param shader_file Ruta relativa del archivo.
     * @param type Tipo de shader procesado (VERTEX, FRAGMENT, GEOMETRY).
     * @return std::string Cadena con el contenido del archivo.
     * @throws std::runtime_error Si el archivo no se pudo abrir.
     */
    std::string read_shader_file(const std::string& shader_file, ShaderType type) const;
};