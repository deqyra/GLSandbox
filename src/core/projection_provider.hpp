#ifndef PROJECTION_PROVIDER_HPP
#define PROJECTION_PROVIDER_HPP

#include <glm/glm.hpp>

class ProjectionProvider
{
    public:
        virtual glm::mat4 getProjectionMatrix() = 0;
};

#endif//PROJECTION_PROVIDER_HPP