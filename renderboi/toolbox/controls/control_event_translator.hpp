#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include "control.hpp"
#include "../interfaces/control_binding_provider.hpp"
#include "../interfaces/action_event_receiver.hpp"

namespace Renderboi
{

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
    ///
    /// @param control Structure of litterals describing the control which 
    /// just processed.
    /// @param action Object describing the action that was performed on the
    /// control.
    /// @param window Pointer to the window on which the control was
    /// captured.
    void _translateAndNotify(const Control& control, Window::Input::Action action, const GLWindowPtr window) const;

    /// @brief Copies controls from a control binding provider into a multimap.
    ///
    /// @param bindingProvider Object to copy the bindings from.
    /// @param destination Map to copy the bindings into.
    static void _ImportControlBindingsIntoMap(
        const ControlBindingProviderPtr<T>& bindingProvider,
        std::multimap<Control, T>& destination
    );

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
    void processKeyboard(
        const GLWindowPtr window, 
        const Window::Input::Key key, 
        const int scancode, 
        const Window::Input::Action action, 
        const int mods
    ) override;

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
    void processMouseButton(
        const GLWindowPtr window, 
        const Window::Input::MouseButton button, 
        const Window::Input::Action action, 
        const int mods
    ) override;
};

template<typename T>
ControlEventTranslator<T>::ControlEventTranslator(
    ControlBindingProviderPtr<T> bindingProvider,
    ActionEventReceiverPtr<T> listener
) :
    _bindingProvider(bindingProvider),
    _listener(listener),
    _controlBindings()
{
    _ImportControlBindingsIntoMap(bindingProvider, _controlBindings);
}

template<typename T>
void ControlEventTranslator<T>::processKeyboard(
    const GLWindowPtr window,
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
)
{
    Control triggeredControl = Control(key);

    _translateAndNotify(triggeredControl, action, window);
}

template<typename T>
void ControlEventTranslator<T>::processMouseButton(
    const GLWindowPtr window,
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{
    const Control triggeredControl = Control(button);

    _translateAndNotify(triggeredControl, action, window);
}

template<typename T>
void ControlEventTranslator<T>::_translateAndNotify(const Control& control, Window::Input::Action action, const GLWindowPtr window) const
{
    using Iter = typename std::multimap<Control, T>::const_iterator;
    const std::pair<Iter, Iter> range = _controlBindings.equal_range(control);
    
    if (action == Window::Input::Action::Release)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->stopAction(window, it->second);
        }
    }
    else // (action == Window::Input::Action::Press)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->triggerAction(window, it->second);
        }
    }    
}

template<typename T>
void ControlEventTranslator<T>::_ImportControlBindingsIntoMap(
    const ControlBindingProviderPtr<T>& bindingProvider,
    std::multimap<Control, T>& destination
)
{
    const std::vector<std::pair<Control, T>> bindings = bindingProvider->getAllBoundControls();
    std::copy(bindings.begin(), bindings.end(), std::inserter(destination, destination.begin()));
}

template<typename T>
using ControlEventTranslatorPtr = std::shared_ptr<ControlEventTranslator<T>>;

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP