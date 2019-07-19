#ifndef CAMERA_H
#define CAMERA_H


#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <cmath>


class Camera
{
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 up;
    glm::vec3 lookingAt;
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 VP;
    bool VPchanged = true;

public:
    Camera();

    // Get stuff.
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    // VP stands for View Project.
    glm::mat4 getVP();

    // Set stuff.
    glm::vec3* setPosition();
    glm::vec3* setRotation();
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);

    void addLocalCoordinats(const glm::vec3 coord);

    void lookAt(glm::vec3 position, glm::vec3 lookingAt, glm::vec3 up);
    void projectionPerspective(float f_fieldOfView, float f_aspectRatio, float f_zNear, float f_zFar);
    void update();
};

#endif // CAMERA_H
