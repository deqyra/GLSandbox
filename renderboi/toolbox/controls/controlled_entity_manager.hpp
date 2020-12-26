#ifndef RENDERBOI__TOOLBOX__CONTROLLED_ENTITY_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLLED_ENTITY_MANAGER_HPP

#include <memory>
#include <type_traits>

#include "control_scheme_manager.hpp"
#include "control_event_translator.hpp"
#include "../interfaces/default_control_scheme_provider.hpp"

/// @brief Eases out management of entities which provide a default control 
/// scheme.
///
/// @tparam T Type of the entity to manage. It must have a public ActionType
/// typedef, and inherit from DefaultControlSchemeProvider<ActionType>, as well
/// as from ActionEventReceiver<ActionType>.
template<typename T,
         typename = std::enable_if_t<std::is_convertible<T*, DefaultControlSchemeProvider<typename T::ActionType>*>::value>,
         typename = std::enable_if_t<std::is_convertible<T*, ActionEventReceiver<typename T::ActionType>*>::value>>
class ControlledEntityManager
{
    private:
        /// @brief Pointer to the managed entity.
        std::shared_ptr<T> _entity;

        /// @brief Scheme manager provided by the entity.
        std::shared_ptr<ControlSchemeManager<typename T::ActionType>> _schemeManager;

        /// @brief Object to translate events to actions and forward them to the
        /// managed entity.
        std::shared_ptr<ControlEventTranslator<typename T::ActionType>> _translator;

    public:
        /// @tparam ArgTypes Types of the arguments to forward to the 
        /// constructor of the managed entity.
        ///
        /// @param args Arguments to forward to the constructor of the managed 
        /// entity.
        template<typename... ArgTypes>
        ControlledEntityManager(ArgTypes&&... args) :
            _entity(std::make_shared<T>(std::forward<ArgTypes>(args)...)),
            _schemeManager(_entity->getDefaultControlScheme()),
            _translator(std::make_shared<ControlEventTranslator<typename T::ActionType>>(std::static_pointer_cast<ControlBindingProvider<typename T::ActionType>>(_schemeManager),
                                                                                         std::static_pointer_cast<ActionEventReceiver<typename T::ActionType>>(_entity)))
        {
        }

        /// @brief Get a pointer to the managed entity.
        ///
        /// @return A pointer to the managed entity.
        std::shared_ptr<T> getEntity()
        {
            return _entity;
        }

        /// @brief Get a pointer to the control scheme manager of the managed 
        /// entity.
        ///
        /// @return A pointer to the control scheme manager of the managed 
        /// entity.
        std::shared_ptr<ControlSchemeManager<typename T::ActionType>> getSchemeManager()
        {
            return _schemeManager;
        }

        /// @brief Get a pointer to the control event translator of the managed
        /// entity.
        ///
        /// @return A pointer to the control event translator of the managed 
        /// entity.
        std::shared_ptr<ControlEventTranslator<typename T::ActionType>> getEventTranslator()
        {
            return _translator;
        }
};

#endif//RENDERBOI__TOOLBOX__CONTROLLED_ENTITY_MANAGER_HPP