#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <renderboi/window/input_processor.hpp>

#include "control.hpp"
#include "control_binding_provider.hpp"
#include "action_event_receiver.hpp"

/// @brief Class to be plugged in directly into the window. Will receive and
/// filter control events, translate them into actions if appropriate, and
/// forward those to their listener.
template<typename T>
class ControlEventTranslator : public InputProcessor
{
    private:
        /// @brief Object which can tell which controls are mapped to which
        /// actions.
        ControlBindingProviderPtr<T> _bindingProvider;

        /// @brief Object which will be notified of which actions were triggered
        /// by their bound controls.
        ActionEventReceiverPtr<T> _listener;

        /// @brief Structure mapping actions to the control they are bound to.
        std::multimap<Control, T> _controlBindings;

        /// @brief Translate a given control into action(s) and forward those to
        /// the listener.
        void translateAndNotify(const Control& control, Window::Input::Action action);

        /// @brief Copies controls from a control binding provider into a multimap.
        ///
        /// @param bindingProvider Object to copy the bindings from.
        /// @param destination Map to copy the bindings into.
        static void importControlBindingsIntoMap(ControlBindingProviderPtr<T> bindingProvider, std::multimap<Control, T>& destination);

    public:
        /// @param bindingProvider Object which can tell which controls are 
        /// mapped to which actions.
        /// @param listener Object which will be notified of which actions 
        /// were triggered by their bound controls.
        ControlEventTranslator(ControlBindingProviderPtr<T> bindingProvider, ActionEventReceiverPtr<T> listener);

        //////////////////////////////////////////////
        ///                                        ///
        /// Methods overridden from InputProcessor ///
        ///                                        ///
        //////////////////////////////////////////////

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse button event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param button Literal describing which button triggered the
        /// event.
        /// @param action Literal describing what action was performed on
        /// the button which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the button event (Ctrl, Shift, etc).
        void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);
};

template<typename T>
ControlEventTranslator<T>::ControlEventTranslator(ControlBindingProviderPtr<T> bindingProvider, ActionEventReceiverPtr<T> listener) :
    _bindingProvider(bindingProvider),
    _listener(listener),
    _controlBindings()
{
    importControlBindingsIntoMap(bindingProvider, _controlBindings);
}

template<typename T>
void ControlEventTranslator<T>::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    Control triggeredControl = {
        .kind = ControlKind::Key,
        .key = key
    };

    translateAndNotify(triggeredControl, action);
}

template<typename T>
void ControlEventTranslator<T>::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    Control triggeredControl = {
        .kind = ControlKind::MouseButton,
        .mouseButton = button
    };

    translateAndNotify(triggeredControl, action);
}

template<typename T>
void ControlEventTranslator<T>::translateAndNotify(const Control& control, Window::Input::Action action)
{
    using Iter = std::multimap<Control, T>::iterator;
    std::pair<Iter, Iter> range = _controlBindings.equal_range(control);
    
    if (action == Window::Input::Action::Release)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->stopAction(it->second);
        }
    }
    else // (action == Window::Input::Action::Press)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->triggerAction(it->second);
        }
    }    
}

template<typename T>
void ControlEventTranslator<T>::importControlBindingsIntoMap(ControlBindingProviderPtr<T> bindingProvider, std::multimap<Control, T>& destination)
{
    std::vector<std::pair<Control, T>> bindings = bindingProvider->getAllBoundControls();
    std::copy(bindings.begin(), bindings.end(), std::inserter(destination, destination.begin()));
}

template<typename T>
using ControlEventTranslatorPtr = std::shared_ptr<ControlEventTranslator<T>>;

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP