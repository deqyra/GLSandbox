#ifndef RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP
#define RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP

#include <memory>

#include <glm/vec3.hpp>

class BasisProvider
{
    public:
        virtual glm::vec3 left() const = 0;
        virtual glm::vec3 up() const = 0;
        virtual glm::vec3 forward() const = 0;
};

using BasisProviderPtr = std::shared_ptr<BasisProvider>;

#endif//RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP