#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP

#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include <cpptools/map_tools.hpp>

#include "control.hpp"

/// @brief Given an enum of actions, this class allows easy management of
/// controls bound to these actions.
///
/// @tparam T Class representing the action to which a control can be bound. 
/// @tparam U Class responsible for hashing T.
template<typename T, typename U = std::hash<T>>
class ControlSchemeManager
{
    protected:
        /// @brief Maximum number of controls bound to a single action in the
        /// ControlSchemeManager.
        const unsigned int _maxControlsPerAction;

        /// @brief Structure mapping controls to the actions they are bound to
        /// (several controls can be bound to a single action).
        std::unordered_multimap<T, Control, U> _controlsBoundToAction;

        /// @brief Structure mapping actions to a control bound to them (a 
        /// control can only be bound to one action in the same scheme).
        std::unordered_map<Control, T, ControlHash> _actionBoundToControl;

    public:
        /// @brief The default maximum number of controls bound to a single
        /// action in the ControlSchemeManager.
        static constexpr unsigned int DefaultMaxControlsPerAction;

        /// @param maxControlsPerAction Maximum number of controls bound to a
        /// single action in the ControlSchemeManager.
        ControlSchemeManager(unsigned int maxControlsPerAction = DefaultMaxControlsPerAction);

        /// @brief Bind a control to an action. A control cannot be bound to
        /// several actions, and a binding of the provided control to another
        /// action will be removed if present. However, several controls can be
        /// bound to the same action, and bindings of other controls to the
        /// provided action will not be removed. If the provided control is
        /// already bound to the provided action, nothing happens.
        /// 
        /// @param control Structure of litterals describing the control to bind.
        /// @param action Object describing the action to bind the control to.
        ///
        /// @exception If binding the provided control to the provided action
        /// would exceed the max number of bindings per action as defined at 
        /// construction, then the function will throw an std::runtime_error.
        void bindControl(Control control, T action);

        /// @brief Tells whether or not a control is bound to an action in this 
        /// control scheme.
        ///
        /// @param control Structure of litterals describing the control whose 
        /// binding to check.
        ///
        /// @return Whether or not the provided control is bound.
        bool controlIsBound(const Control& control);

        /// @brief Returns the action bound to the provided control.
        ///
        /// @param control Structure of litterals describing the control bound
        /// to the actions to be retrieved.
        ///
        /// @return The array of actions to which the provided control is bound.
        ///
        /// @exception If the provided control is not bound in this control 
        /// scheme, the function will throw an std::runtime_error.
        T getActionBoundToControl(const Control& control);

        /// @brief Tells whether or not an action has a control bound to it in 
        /// this control scheme.
        ///
        /// @param action Object describing the action whose bindings to check.
        ///
        /// @return Whether or not the provided action has a binding.
        bool actionIsBound(const T& action);

        /// @brief Returns the array of controls which are bound to the provided
        /// action.
        ///
        /// @param action Object describing the action whose bound controls must
        /// be retrieved.
        ///
        /// @return The array of controls bound to the provided action.
        std::vector<Control> getControlsBoundToAction(const T& action);

        /// @brief Returns the array of all controls which are bound to an 
        /// action, paired with the action they are bound to.
        ///
        /// @return The array of all controls bound to an action.
        std::vector<std::pair<Control, T>> getAllBoundControls();

        /// @brief Unbind a provided control in the control scheme.
        ///
        /// @param control Structure of litterals describing the control to
        /// unbind.
        void unbindControl(Control control);

        /// @brief Unbind all controls from a provided action in the control
        /// scheme.
        ///
        /// @param action Object describing the action to unbind controls
        /// from.
        ///
        /// @return The amount of controls that were unbound.
        unsigned int unbindAllControlsFromAction(const T& action);
};

template<typename T, typename U>
constexpr unsigned int ControlSchemeManager<T, U>::DefaultMaxControlsPerAction = 4;

template<typename T, typename U>
ControlSchemeManager<T, U>::ControlSchemeManager(unsigned int maxControlsPerAction) :
    _maxControlsPerAction(maxControlsPerAction),
    _controlsBoundToAction(),
    _actionBoundToControl()
{
    
}

template<typename T, typename U>
void ControlSchemeManager<T, U>::bindControl(Control control, T action)
{
    unsigned int actionBindingCount = _controlsBoundToAction.count(action);
    bool alreadyPresent = mapContainsPair(_actionBoundToControl, {control, action});

    if (actionBindingCount >= _maxControlsPerAction)
    {
        if (!alreadyPresent)
        {
            std::string s = "ControlSchemeManager: cannot bind control " + std::to_string(control)
                        + " to action " + std::to_string(action) + ", as it already has "
                        "the max number of bindings (" + std::to_string(_maxControlsPerAction) + ").";
            throw std::runtime_exception(s.c_str());
        }
        else return;
    }

    if (!alreadyPresent)
    {
        _actionBoundToControl.insert({control, action});
        _controlsBoundToAction.insert({action, control});
    }
}

template<typename T, typename U>
bool ControlSchemeManager<T, U>::controlIsBound(const Control& control)
{
    return _actionBoundToControl.contains(control);
}

template<typename T, typename U>
T ControlSchemeManager<T, U>::getActionBoundToControl(const Control& control)
{
    if (!_actionBoundToControl.contains(control))
    {
        std::string s = "ControlSchemeManager: control " + std::to_string(control) + " is not bound, cannot retrieve action.";
        throw std::runtime_error(s.c_str());
    }

    return _actionBoundToControl.at(control);
}

template<typename T, typename U>
bool ControlSchemeManager<T, U>::actionIsBound(const T& action)
{
    return _controlsBoundToAction.contains(action);
}

template<typename T, typename U>
std::vector<Control> ControlSchemeManager<T, U>::getControlsBoundToAction(const T& action)
{
    using Iter = std::unordered_multimap<T, Control, U>::iterator;
    std::pair<Iter, Iter> range = _controlsBoundToAction.equal_range(action);

    std::vector<Control> controls;
    for (Iter it = range.first; it != range.second; it++)
    {
        controls.push_back(it->second);
    }

    return controls;
}

template<typename T, typename U>
std::vector<std::pair<Control, T>> ControlSchemeManager<T, U>::getAllBoundControls()
{
    std::vector<std::pair<Control, T>> boundControls;
    std::copy(_actionBoundToControl.begin(), _actionBoundToControl.end(), std::back_inserter(boundControls));

    return boundControls;
}

template<typename T, typename U>
void ControlSchemeManager<T, U>::unbindControl(Control control)
{
    if (!mapContainsPair(_actionBoundToControl, {control, action})) return;

    _actionBoundToControl.erase(control);

    using Iter = std::unordered_multimap<T, Control, U>::iterator;
    std::pair<Iter, Iter> range = _controlsBoundToAction.equal_range(action);
    for (Iter it = range.first; it != range.second; it++)
    {
        if (it->second == control)
        {
            _controlsBoundToAction.erase(it);
            return;
        }
    }
}

template<typename T, typename U>
unsigned int ControlSchemeManager<T, U>::unbindAllControlsFromAction(const T& action)
{
    if (_controlsBoundToAction.count(action) == 0) return 0;

    using Iter = std::unordered_multimap<T, Control, U>::iterator;
    std::pair<Iter, Iter> range = _controlsBoundToAction.equal_range(action);
    for (Iter it = range.first; it != range.second; it++)
    {
        _actionBoundToControl.erase(it->second);
    }

    return (unsigned int) _controlsBoundToAction.erase(action);
}

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP