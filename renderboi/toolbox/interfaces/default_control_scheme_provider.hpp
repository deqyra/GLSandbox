#ifndef RENDERBOI__TOOLBOX__INTERFACES__DEFAULT_CONTROL_SCHEME_PROVIDER_HPP
#define RENDERBOI__TOOLBOX__INTERFACES__DEFAULT_CONTROL_SCHEME_PROVIDER_HPP

#include <memory>

#include "../controls/control_scheme_manager.hpp"

template<typename T>
class DefaultControlSchemeProvider
{
public:
    /// @brief Get the default control scheme for the implementing entity.
    ///
    /// @return The default control scheme for the implementing entity.
    virtual ControlSchemeManagerPtr<T> getDefaultControlScheme() const = 0;
};

#endif//RENDERBOI__TOOLBOX__INTERFACES__DEFAULT_CONTROL_SCHEME_PROVIDER_HPP