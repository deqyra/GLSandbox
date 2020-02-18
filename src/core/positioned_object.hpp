/**
    GLTest, positioned_object.hpp
    Purpose: Define an abstract class representing an object with a position and orientation.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#ifndef POSITIONED_OBJECT_HPP
#define POSITIONED_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define WORLD_X glm::vec3(1.f, 0.f, 0.f)
#define WORLD_Y glm::vec3(0.f, 1.f, 0.f)
#define WORLD_Z glm::vec3(0.f, 0.f, 1.f)

class PositionedObject
{
    protected:
        glm::vec3 _position;
        glm::quat _orientation;
        glm::vec3 _scale;

        glm::mat4 _modelMatrix;

        bool _matricesOutdated;

        void updateMatrices();

    public:
        PositionedObject();
        PositionedObject(glm::vec3 position, glm::quat orientation, glm::vec3 scale);

        glm::vec3 getPosition();
        void setPosition(glm::vec3 position);
        glm::vec3 translate(glm::vec3 translation);
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        glm::quat getOrientation();
        void setOrientation(glm::quat orientation);
        glm::quat rotate(glm::quat rotation);
        glm::quat rotate(float radAngle, glm::vec3 axis);
        glm::quat lookAt(glm::vec3 target);

        glm::vec3 getScale();
        void setScale(glm::vec3 scale);
        glm::vec3 scale(glm::vec3 scaling);

        glm::mat4 getModelMatrix();
};

#endif//POSITIONED_OBJECT_HPP