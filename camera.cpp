#include "camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() :
    position(0.0f),
    rotation(0.0f)
{

}

glm::vec3 Camera::getPosition()
{
    return this->position;
}

glm::vec3 Camera::getRotation()
{
    return this->rotation;
}
glm::vec3* Camera::setPosition()
{
    this->VPchanged = true;
    return &this->position;
}

glm::vec3* Camera::setRotation()
{
    this->VPchanged = true;
    return &this->rotation;
}

void Camera::setPosition(glm::vec3 position)
{
    this->VPchanged = true;
    this->position = position;
}

void Camera::setRotation(glm::vec3 rotation)
{
    this->VPchanged = true;
    this->rotation = rotation;
}

void Camera::addLocalCoordinats(const glm::vec3 coord)
{
    glm::vec3 test;
    test = glm::rotate(coord, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    //    test = glm::rotate(test, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    test = glm::rotate(test, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    position += test;
    this->VPchanged = true;
}

void Camera::lookAt(glm::vec3 position, glm::vec3 lookingAt, glm::vec3 up)
{
    this->VPchanged = true;
    this->position = position;
    this->up = up;
    this->lookingAt = lookingAt;

    this->View = glm::lookAt(this->position, this->lookingAt, this->up);
}

void Camera::projectionPerspective(float f_fieldOfView, float f_aspectRatio, float f_zNear, float f_zFar)
{
    this->VPchanged = true;
    this->Projection =glm::perspective(f_fieldOfView, f_aspectRatio, f_zNear, f_zFar);
}

glm::mat4 Camera::getVP()
{
    return this->VP;
}

void Camera::update()
{
    if(this->VPchanged)
    {
        this->lookingAt = glm::vec3(position.x-sin(rotation.z*M_PI/180.0f),
                                    (position.y+cos(rotation.z*M_PI/180.0f)),
                                    position.z+tan(rotation.y*M_PI/180.0f));


        this->VP = this->Projection * this->View;
        VPchanged = false;
    }
}
