#ifndef RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP
#define RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP

#include <memory>

#include <glm/vec3.hpp>

namespace Renderboi
{

/// @brief Interface for a class able to provide basis vectors.
class BasisProvider
{
public:
    /// @brief Get the X vector of the basis, potentially in terms of a 
    /// different basis.
    ///
    /// @return X vector of the basis.
    virtual glm::vec3 left() const = 0;

    /// @brief Get the Y vector of the basis, potentially in terms of a 
    /// different basis.
    ///
    /// @return Y vector of the basis.
    virtual glm::vec3 up() const = 0;

    /// @brief Get the Z vector of the basis, potentially in terms of a 
    /// different basis.
    ///
    /// @return Z vector of the basis.
    virtual glm::vec3 forward() const = 0;
};

using BasisProviderPtr = std::shared_ptr<BasisProvider>;

}//namespace Renderboi

#endif//RENDERBOI__CORE__INTERFACES__BASIS_PROVIDER_HPP