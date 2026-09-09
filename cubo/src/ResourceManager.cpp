/**
 * @file ResourceManager.cpp
 * @brief Implementación de la clase ResourceManager.
 * 
 * Esta clase se encarga de gestionar la carga y almacenamiento de recursos de shaders.
 * Proporciona funciones para cargar shaders desde archivos, almacenarlos en un mapa y
 * recuperarlos mediante claves autoincrementales.
 * @warning Esta clase no es thread-safe. Se recomienda usarla en un solo hilo o implementar
 */

#include "ResourceManager.hpp"
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const std::filesystem::path& assets_root)
    : assets_root_(assets_root), next_key_(0) {}


std::string ResourceManager::read_shader_file(const std::string& shader_file, ShaderType type) const {
    // Si la cadena está vacía, no intenta leer (caso del Geometry Shader opcional)
    if (shader_file.empty()) {
        return "";
    }

    std::filesystem::path full_path = assets_root_ / shader_file;
    std::ifstream file(full_path);

    std::string type_str;
    switch (type) {
        case ShaderType::VERTEX:   type_str = "VERTEX"; break;
        case ShaderType::FRAGMENT: type_str = "FRAGMENT"; break;
        case ShaderType::GEOMETRY: type_str = "GEOMETRY"; break;
    }

    if (!file.is_open()) {
        throw std::runtime_error("ERROR::RESOURCE_MANAGER: No se pudo abrir el archivo de " +
                                 type_str + " SHADER en la ruta: " + full_path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const ShaderSource& ResourceManager::load_shader_source(int& key,
                                                       const std::string& vs_file,
                                                       const std::string& fs_file,
                                                       const std::string& gs_file) {

    // Cargar y leer los archivos de shaders
    ShaderSource source;
    source.vs = read_shader_file(vs_file, ShaderType::VERTEX);
    source.fs = read_shader_file(fs_file, ShaderType::FRAGMENT);
    source.gs = read_shader_file(gs_file, ShaderType::GEOMETRY);

    // Generar el nuevo ID autoincremental
    int assigned_id = next_key_++;

    // Sobrescribir incondicionalmente la variable externa apuntada por 'key'
    key = assigned_id;

    // Asignar los datos en el mapa con esa clave generada
    shaders_sources_[assigned_id] = std::move(source);

    return shaders_sources_[assigned_id];
}

const ShaderSource& ResourceManager::get_shader_source(int key) const {
    auto it = shaders_sources_.find(key);
    
    if (it == shaders_sources_.end()) {
        throw std::out_of_range("ERROR::RESOURCE_MANAGER: Intento de acceso invalido. La clave " +
                                std::to_string(key) + " no existe.");
    }

    return it->second;
}

void ResourceManager::clear() {
    shaders_sources_.clear();
    next_key_ = 0;
}