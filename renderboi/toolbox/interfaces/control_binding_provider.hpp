#ifndef CONTROL_BINDING_PROVIDER_HPP
#define CONTROL_BINDING_PROVIDER_HPP

#include <memory>

template<typename T>
class ControlBindingProvider
{
public:
    /// @brief Tells whether or not a control is bound to an action in this 
    /// control scheme.
    ///
    /// @param control Structure of litterals describing the control whose 
    /// binding to check.
    ///
    /// @return Whether or not the provided control is bound.
    virtual bool controlIsBound(const Control& control) = 0;

    /// @brief Returns the action bound to the provided control.
    ///
    /// @param control Structure of litterals describing the control bound
    /// to the actions to be retrieved.
    ///
    /// @return The array of actions to which the provided control is bound.
    ///
    /// @exception If the provided control is not bound in this control
    /// scheme, depending on how the concrete function is implemented, it 
    /// may throw an std::runtime_error.
    virtual T getActionBoundToControl(const Control& control) = 0;

    /// @brief Tells whether or not an action has a control bound to it in 
    /// this control scheme.
    ///
    /// @param action Object describing the action whose bindings to check.
    ///
    /// @return Whether or not the provided action has a binding.
    virtual bool actionIsBound(const T& action) = 0;

    /// @brief Returns the array of controls which are bound to the provided
    /// action.
    ///
    /// @param action Object describing the action whose bound controls must
    /// be retrieved.
    ///
    /// @return The array of controls bound to the provided action.
    virtual std::vector<Control> getControlsBoundToAction(const T& action) = 0;

    /// @brief Returns the array of all controls which are bound to an 
    /// action, paired with the action they are bound to.
    ///
    /// @return The array of all controls bound to an action.
    virtual std::vector<std::pair<Control, T>> getAllBoundControls() = 0;
};

template<typename T>
using ControlBindingProviderPtr = std::shared_ptr<ControlBindingProvider<T>>;

#endif//CONTROL_BINDING_PROVIDER_HPP