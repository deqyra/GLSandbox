#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP

#include <functional>

#include "control.hpp"

/// @brief Given an enum of actions, this class allows easy management of
/// controls bound to these actions.
///
/// @tparam T Object representing the action to which a control can be bound. 
/// @tparam U Class responsible for hashing T.
template<typename T, typename U = std::hash<T>>
class ControlSchemeManager
{
    protected:
        /// @brief Structure mapping actions to the keys they are bound to.
        std::unordered_multimap<T, Control, U> _keysBoundToAction;

        /// @brief Structure mapping keys to the actions they are bound to.
        std::unordered_multimap<Control, T, ControlHash> _actionsBoundToKey;

    public:
        /// @brief Bind a control to an action. A control can be bound to
        /// several actions, and bindings of this control to other actions will
        /// not be removed if any exists.
        /// 
        /// @param control Structure of litterals describing the control to bind.
        /// @param action Object describing the action to bind the control to.
        void bindControl(Control control, T action);

        /// @brief Tells whether or not a control is bound to an action in this 
        /// control scheme.
        ///
        /// @param control Structure of litterals describing the control whose 
        /// bindings to check.
        ///
        /// @return Tells whether or not the provided control is bound.
        bool controlIsBound(Control control);

        /// @brief Returns an array of actions which are bound to the provided
        /// control.
        ///
        /// @param control Structure of litterals describing the control whose
        /// bound actions must be retrieved.
        ///
        /// @return An array of actions which are bound to the provided control.
        std::vector<T> getActionsBoundToControl(Key key);
};

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_MANAGER_HPP