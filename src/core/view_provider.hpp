/**
    GLTest, view_provider.hpp
    Purpose: Interface for a class which can give off a view matrix.

    @author François Brachais (deqyra)
    @version 1.0 11/09/2019
 */

#ifndef VIEW_PROVIDER_HPP
#define VIEW_PROVIDER_HPP

#include <glm/glm.hpp>

class ViewProvider
{
    public:
        virtual glm::mat4 getViewMatrix() = 0;
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition) = 0;
};

#endif//VIEW_PROVIDER_HPP