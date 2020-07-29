//
// Created by kepler-br on 6/11/20.
//

#ifndef WOLFENSTEIN_TYPES_H
#define WOLFENSTEIN_TYPES_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum Directions {
    direction_right = 0,
    direction_up,
    direction_left,
    direction_down
};

struct Sided_texture_ids
{
    size_t right;
    size_t up;
    size_t left;
    size_t down;
};

struct Block {
    glm::ivec2 world_position;
    glm::vec2 scaled_world_position;
    glm::ivec3 color;
    bool is_solid_wall;
    bool is_transparent;
    bool is_passable;
    bool is_portal;
    bool is_lit_by_lamp;
    bool seen_by_player;
    bool is_ceiling_sky;
    bool is_bottom_sky;
    size_t portal_to_block_id;
    size_t top_sky_texture_id;
    size_t bottom_sky_texture_id;
    Sided_texture_ids wall_textures;
    size_t floor_texture_id;
    size_t ceiling_texture_id;
    size_t floor_height;
    size_t ceiling_height;
};

struct Ray
{
    size_t block_id;
    glm::ivec2 position;
    float length;
    Directions hit_side;
};

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif //WOLFENSTEIN_TYPES_H
