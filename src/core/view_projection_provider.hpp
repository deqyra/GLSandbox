#ifndef VIEW_PROJECTION_PROVIDER_HPP
#define VIEW_PROJECTION_PROVIDER_HPP

#include "view_provider.hpp"
#include "projection_provider.hpp"

#include <glm/glm.hpp>

class ViewProjectionProvider : public ViewProvider, public ProjectionProvider
{
    public:
        virtual ~ViewProjectionProvider();
        virtual glm::mat4 getViewMatrix() = 0;
        virtual glm::mat4 getViewMatrix(glm::vec3 position) = 0;
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition) = 0;
        virtual glm::mat4 getProjectionMatrix() = 0;
        virtual glm::mat4 getViewProjectionMatrix() = 0;
};

#endif//VIEW_PROJECTION_PROVIDER_HPP