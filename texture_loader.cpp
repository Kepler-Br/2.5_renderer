#include "texture_loader.h"

void Texture_loader::throw_error(int status, const std::string &path)
{
//    if(status == Z_STREAM_END)
//        throw std::runtime_error("Texture uncompression: stream end: " + path);
    if(status == Z_NEED_DICT)
        throw std::runtime_error("Texture uncompression: need dict: " + path);
    if(status == Z_ERRNO)
        throw std::runtime_error("Texture uncompression: errno: " + path);
    if(status == Z_STREAM_ERROR)
        throw std::runtime_error("Texture uncompression: stream error: " + path);
    if(status == Z_DATA_ERROR)
        throw std::runtime_error("Texture uncompression: data error: " + path);
    if(status == Z_MEM_ERROR)
        throw std::runtime_error("Texture uncompression: memory error: " + path);
    if(status == Z_BUF_ERROR)
        throw std::runtime_error("Texture uncompression: buffer error: " + path);
    if(status == Z_VERSION_ERROR)
        throw std::runtime_error("Texture uncompression: version error: " + path);
}

std::string Texture_loader::get_filename_from_path(const std::string &path)
{
    static std::regex pattern(R"(([\w\d\s\!@#$%\^&\*\(\)-\+=\";\:\'\{\}\[\]<>\?]+)\.[\w]+$)");
    std::smatch match;
    std::regex_search(path, match, pattern);
    return match[1];
}

uint8_t *Texture_loader::read_compressed_data(std::ifstream &stream, const uint32_t &size)
{
    uint8_t *data;
    data = new uint8_t[size];
    stream.read((char *)data, size);
    return data;
}

uint8_t *Texture_loader::uncompress_data(uint8_t *data, const uint32_t &uncompressed_size, const uint32_t &compressed_size, const std::string &original_file_path)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = compressed_size;
    stream.next_in = data;
    uint8_t *uncompressed_data = new uint8_t[uncompressed_size];
    stream.avail_out = uncompressed_size;
    stream.next_out = uncompressed_data;
    inflateInit(&stream);
    int ret = inflate(&stream, Z_FINISH);
    if (ret != Z_OK)
        this->throw_error(ret, original_file_path);
    inflateEnd(&stream);
    return uncompressed_data;
}

Texture *Texture_loader::setup_texture(uint8_t *data, const glm::ivec2 &resolution, const bool has_alpha)
{
    uint pixel_count = resolution.x * resolution.y;
//    uint data_size;
//    if(has_alpha)
//        data_size = pixel_count * 4 * sizeof(uint8_t);
//    else
//        data_size = pixel_count * 3 * sizeof(uint8_t);
    Texture *new_texture = new Texture;

    new_texture->transparent = has_alpha;
    new_texture->resolution = resolution;
    new_texture->pixels = new Pixel[pixel_count];
    new_texture->pixel_count = pixel_count;
    for(int x = 0; x < resolution.x; x++)
    {
        for(int y = 0; y < resolution.y; y++)
        {
            const uint index = y * resolution.x + x;
            Pixel pixel;
            if(has_alpha)
            {
                uint8_t *pointer = &data[index*4];
                pixel = {pointer[0], pointer[1], pointer[2], pointer[3]};
            }
            else
            {
                uint8_t *pointer = &data[index*3];
                pixel = {pointer[0], pointer[1], pointer[2], 255};
            }
            new_texture->set_pixel(pixel, glm::ivec2{x, y});
        }
    }
    return new_texture;
}

Texture_loader::Texture_loader()
{

}

std::pair<Texture *, std::string> Texture_loader::load(const std::string &path)
{
    std::ifstream stream;
    stream.open(path);
    if(stream.bad() || !stream.is_open())
        throw std::runtime_error("Cannot open image: " + path);
    uint8_t file_sign[3];
    stream.read((char *)file_sign, 3);
    if(file_sign[0] != 'X' || file_sign[1] != 'F' || file_sign[2] != 'T')
        throw std::runtime_error("Trying to open image that is not an image(No XFT magic number): " + path);
    uint32_t width;
    uint32_t height;
    stream.read((char *)&width, sizeof(uint32_t));
    stream.read((char *)&height, sizeof(uint32_t));
    bool has_alpha;
    stream.read((char *)&has_alpha, sizeof(bool));
    uint32_t raw_texture_size;
    stream.read((char *)&raw_texture_size, sizeof(uint32_t));
    uint32_t compressed_size;
    stream.read((char *)&compressed_size, sizeof(uint32_t));
    uint8_t *compressed_data = this->read_compressed_data(stream, compressed_size);
    uint8_t *uncompressed_data = this->uncompress_data(compressed_data, raw_texture_size, compressed_size, path);
    delete[] compressed_data;
    Texture *texture = this->setup_texture(uncompressed_data, glm::ivec2(width, height), has_alpha);
    delete[] uncompressed_data;
    const std::string texture_name = this->get_filename_from_path(path);
    return std::make_pair(texture, texture_name);
}
