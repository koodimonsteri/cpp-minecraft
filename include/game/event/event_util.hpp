#pragma once

#include "event_handler.hpp"
#include "event.hpp"

template <typename... T>
void bindEvent(EventType type, Event<T...> event) {
    EventHandler<T...>::bindEvent(type, event);
}

template<typename... T>
void dispatch(Event<T...> event) {
    EventHandler<T...>::dispatch(event);
}