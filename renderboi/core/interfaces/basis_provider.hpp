#ifndef RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP
#define RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP

#include <memory>

#include <glm/vec3.hpp>

class BasisProvider
{
    public:
        virtual glm::vec3 left() = 0;
        virtual glm::vec3 up() = 0;
        virtual glm::vec3 forward() = 0;
};

using BasisProviderPtr = std::shared_ptr<BasisProvider>;

#endif//RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP