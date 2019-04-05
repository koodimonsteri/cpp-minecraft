#pragma once

#include <vector>
#include <functional>
#include <map>
#include "event.hpp"

template <typename... T>
using Action = std::function<void(T...)>;

// EventHandlers are used for
// - Binding Actions to EventTypes
// - Binding Events to EventTypes
// - Dispatching events
// - Storing connections between EventTypes and Actions
template <typename... T>
class EventHandler {
    public:
        // Binds an Action to an EventType
        // 'action' will be called whenever an event of type 'type' gets dispatched.
        // erases previous bindings
        static void bindAction(EventType type, Action<T...> action) {
            actions_[type].clear();
            appendAction(type, action);
        };

        // Binds an Action to an EventType
        // 'action' will be called whenever an event of type 'type' gets dispatched.
        static void appendAction(EventType type, Action<T...> action) {
            actions_[type].push_back(action);
        }

        // Dispatches an event
        // Actions that are bound to the type of the 'event' will be called.
        static void dispatch(Event<T...> event) {
            try {
                for (auto action : actions_[event.getType()]) {
                    std::apply(action, event.getParams()); // Due to performance reasons, existance of the key is not checked.
                }
            } catch (std::bad_function_call e) {
                throw std::runtime_error("Dispatched an event without a defined action.");
            }
        }

    public:
        static std::map<EventType, std::vector<Action<T...>>> actions_;
};

template <typename... T>
std::map<EventType, std::vector<Action<T...>>> EventHandler<T...>::actions_;