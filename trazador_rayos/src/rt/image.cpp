#include <cstdint>

#include <image.h>

// stb no compila limpio con -Wextra: se silencia ese aviso sólo para este include
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#pragma GCC diagnostic pop

bool Image::write(const std::string& path, float gamma) const
{
    // De color lineal a color de 8 bits por componente, con la corrección gamma
    std::vector<std::uint8_t> bytes(pixels.size() * 3);
    for( size_t k = 0; k < pixels.size(); ++k ){
        for( int c = 0; c < 3; ++c ){
            const float lin = glm::clamp(pixels[k][c], 0.0f, 1.0f);
            const float enc = (lin > 0.0f) ? glm::pow(lin, 1.0f / gamma) : 0.0f;
            bytes[3 * k + size_t(c)] = std::uint8_t(255.999f * enc);
        }
    }

    // stride: bytes por fila
    return stbi_write_png(path.c_str(), W, H, 3, bytes.data(), W * 3) != 0;
}
