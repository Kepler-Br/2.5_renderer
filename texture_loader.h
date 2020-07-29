#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "texture.h"
#include <zlib.h>
#include <fstream>
#include <regex>
#include <iostream> // DELETEME

class Texture_loader
{
    void throw_error(int status, const std::string &path);
    std::string get_filename_from_path(const std::string &path);
    uint8_t *read_compressed_data(std::ifstream &stream, const uint32_t &size);
    uint8_t *uncompress_data(uint8_t *data, const uint32_t &uncompressed_size, const uint32_t &compressed_size, const std::string &original_file_path);
    Texture *setup_texture(uint8_t *data, const glm::ivec2 &resolution, const bool has_alpha);

public:
    Texture_loader();
    // Return: Texture and texture name.
    std::pair<Texture *, std::string> load(const std::string &path);
};

#endif // TEXTURE_LOADER_H
