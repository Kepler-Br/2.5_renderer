#include "texture_holder.h"

Texture_holder::Texture_holder(const int &reserve_texture_count)
{
    this->textures.reserve(reserve_texture_count);
}

Texture_holder::Texture_holder()
{
    this->textures.reserve(10);
}

Texture_holder::~Texture_holder()
{
    for(auto *texture : this->textures)
    {
        texture->free();
        delete texture;
    }
}

int Texture_holder::get_total() const
{
    return this->textures.size();
}

Texture *Texture_holder::get_by_id(const size_t &id) const
{
    if (id >= this->textures.size())
        throw std::runtime_error("Tried to access non existing texture by id.");
    return this->textures[id];
}

Texture *Texture_holder::get_by_name(const std::string &name) const
{
    if(this->texture_names.find(name) == this->texture_names.end())
        throw std::runtime_error("Tried to access non existing texture by name.");
    return this->texture_names.at(name).first;
}

int Texture_holder::load(const std::string &path, const std::string &name)
{
    auto *texture = new Texture;
    texture->read(path);
    this->textures.push_back(texture);
    if(this->texture_names.find(name) != this->texture_names.end())
        throw std::runtime_error("Tried to add existing texture to texture names.");
    this->texture_names[name] = std::make_pair(texture, this->textures.size() - 1);
    return (int)this->textures.size() - 1;
}

void Texture_holder::unload_all()
{
    for(auto &texture : this->textures)
        texture->free();
    this->textures.clear();
    this->texture_names.clear();
}

void Texture_holder::unload(const size_t &id)
{
    if(id >= this->textures.size())
        throw std::runtime_error("Tried to unload not existing texture by id.");
    this->textures[id]->free();

    this->textures.erase(this->textures.begin() + id);
    for(auto &map_iter : this->texture_names)
    {
        if(map_iter.second.second == id)
        {
            this->texture_names.erase(map_iter.first);
            break;
        }
    }
}

void Texture_holder::unload(const std::string &name)
{
    const auto &tex = this->texture_names.find(name);
    if(tex == this->texture_names.end())
        throw std::runtime_error("Tried to unload not existing texture by name.");
    tex->second.first->free();
    int id = tex->second.second;
    this->textures.erase(this->textures.begin() + id);
    this->texture_names.erase(tex);
}
