#ifndef CORE__PLAIN_TRANSFORM_HPP
#define CORE__PLAIN_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// An object that has 3D-space properties : position, orientation and scale
class PlainTransform
{
    protected:
        // 3D position of the object
        glm::vec3 _position;
        // Quaternion representing the rotation of the object
        glm::quat _orientation;
        // 3D scale of the object
        glm::vec3 _scale;

        // Model matrix of the object
        glm::mat4 _modelMatrix;

        // Whether the model matrix no longer reflects the transform parameters
        bool _matrixOutdated;

        // Update the model matrix of the object so that it reflects the transform parameters
        void updateMatrix();

    public:
        static constexpr glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
        static constexpr glm::vec3 X = glm::vec3(1.f, 0.f, 0.f);
        static constexpr glm::vec3 Y = glm::vec3(0.f, 1.f, 0.f);
        static constexpr glm::vec3 Z = glm::vec3(0.f, 0.f, 1.f);

        PlainTransform();
        PlainTransform(glm::vec3 position, glm::quat orientation, glm::vec3 scale);

        // Get the position of the object
        glm::vec3 getPosition();
        // Set the position of the object
        void setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector
        glm::vec3 translate(glm::vec3 translation);
        // Orbit the object around an axis and center
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object
        glm::quat getOrientation();
        // Set the orientation of the object
        void setOrientation(glm::quat orientation);
        // Rotate the object by a quaternion
        glm::quat rotate(glm::quat rotation);
        // Rotate the object around an axis
        glm::quat rotate(float radAngle, glm::vec3 axis, bool localAxis = false);
        // Rotate the object so that its front is directed to the target position
        glm::quat lookAt(glm::vec3 target);

        // Get the scale of the object
        glm::vec3 getScale();
        // Set the scale of the object
        void setScale(glm::vec3 scale);
        // Scale the object by an amount
        glm::vec3 scale(glm::vec3 scaling);

        // Get the model matrix of the object
        glm::mat4 getModelMatrix();
};

#endif//CORE__PLAIN_TRANSFORM_HPP