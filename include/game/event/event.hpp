#pragma once

#include <string>


enum class EventType {
    INIT_APPLICATION, INIT_WINDOW, INIT_LOGIC,
    APPLICATION_CLOSE,
    E_KEY, E_KEY_PRESS, E_KEY_RELEASE,
    E_MOUSE, E_MOUSE_PRESS, E_MOUSE_RELEASE,
    UPDATE_RENDERABLE, DELETE_RENDERABLE, UPDATE_CHUNK_RENDERABLE, DELETE_CHUNK_RENDERABLE,
    WINDOW_RESIZE,
    PLAYER_MOVE, PLAYER_PUNCH,
    PRIMARY_ACTION, SECONDARY_ACTION,
    BIND_PLAYER, BIND_INVENTORY,
    CHANGE_MODE,
    UPDATE_VIEW_PROJECTION, UPDATE_MODEL, UPDATE_TEXTURE, UPDATE_VIEW_MATRIX
};

template <typename... T>
class Event {
    public:
      Event(EventType type, T... params) : type_(type), params_(std::forward_as_tuple(params...)) {};
      EventType        getType() { return type_; };
      std::tuple<T...> getParams() { return params_; }

    private:
        EventType type_;
        std::tuple<T...> params_;
};
