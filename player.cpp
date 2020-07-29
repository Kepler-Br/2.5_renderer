#include "player.h"

int Player::get_height() const
{
    return height;
}

int Player::get_speed() const
{
    return speed;
}

//void Player::calculate_forward()
//{
//    this->forward = {Lookup_table::cos(this->x_view_angle),
//                     Lookup_table::sin(this->x_view_angle)};
//}

int Player::get_running_speed() const
{
    return running_speed;
}

void Player::setRunning_speed(int value)
{
    running_speed = value;
}

void Player::setSpeed(int value)
{
    speed = value;
}

void Player::set_height(int value)
{
    height = value;
}

int Player::clamp_angle(const int degree)
{
    //    if(degree >= 360)
//        return degree % 360;
//    else if (degree < 0)
//        return degree % 360 + 360;
    return degree;
}

Player::Player(const glm::ivec2 &position, const int &look_angle):
    x_view_angle(look_angle),
    position(position)
{
//    this->calculate_forward();
}

void Player::set_x_view_angle(const int &degree)
{
    this->x_view_angle = this->clamp_angle(degree);
//    this->calculate_forward();
}

void Player::add_x_view_angle(const int &degree)
{
    this->x_view_angle = this->clamp_angle(this->x_view_angle + degree);
//    this->calculate_forward();
}

float Player::get_x_view_angle() const
{
    return this->x_view_angle;
}

void Player::set_y_view_angle(const float &degree)
{
    this->y_view_angle = degree;
}

void Player::add_y_view_angle(const float &degree)
{
    this->y_view_angle = this->y_view_angle + degree;
}

float Player::get_y_view_angle() const
{
    return this->y_view_angle;
}

const glm::vec2 &Player::get_position() const
{
    return this->position;
}

void Player::set_position(const glm::vec2 &position)
{
    this->position = position;
}

void Player::add_position(const glm::vec2 &position)
{
    this->position.x += position.x;
    this->position.y += position.y;
}

int Player::get_distance_to_projection_plane() const
{
    return distance_to_projection_plane;
}
