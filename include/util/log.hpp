#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdlib.h>

#include "glm/glm.hpp"
#include "game/player.hpp"
#include "world/chunk.hpp"

// Macros to simplify logging
#define LOG_TRACE(...)    ::Log::trace(__VA_ARGS__)
#define LOG_INFO(...)     ::Log::info(__VA_ARGS__)
#define LOG_WARN(...)     ::Log::warn(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Log::critical(__VA_ARGS__)

// implement << operator overload here for any object you want to log
inline std::ostream& operator<<(std::ostream& out, const glm::vec3 &vec)     // glm::vec3
{
    out << "vec3(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const glm::vec2& vec)     // glm::vec2
{  
    out << "vec2(" << vec.x << "," << vec.y << ")";
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const glm::ivec3 &vec)    // glm::ivec3
{ 
    out << "ivec3(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const glm::ivec2& vec)    // glm::ivec2
{ 
    out << "ivec2(" << vec.x << "," << vec.y << ")";
    return out;
}

inline std::ostream& operator<<(std::ostream& os, const Player& p)           // Player
{
    os << "-Player { pos: " << p.getPosition() << " rot: " << p.getFacing() << " }";
    return os;
}

/*
 * Simple logging class outputs data to standard output
 * Logging function arguments MUST implement << operator
 * For now functions are implemented as inline to avoid multiple definitions compile error.
 * TODO: either thread-safe logging or own loggerthread.
 */

namespace Log {

    // Returns current time in format: "[HOURS:MINUTES:SECONDS]"
    inline std::string getTimeAsString()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << "[";
        ss << std::put_time(std::localtime(&in_time_t), "%X");
        ss << "]";
        return ss.str();
    }

    // Helper function to handle variable amount of printable arguments
    template<typename C>
    inline void my_cout(C coutable) 
    {
        std::cout << " " << coutable;
    }

    template<typename C, typename... Args>
    inline void my_cout(C coutable, Args... args) 
    {
        std::cout << " " << coutable;
        my_cout(args...);
    }

    #ifdef _WIN32
    #include "windows.h"

    static HANDLE con_handle = GetStdHandle(STD_OUTPUT_HANDLE);  // Console handler for windows
    /*
     * params:
     * 2  Green   (info)
     * 4  Red     (critical)
     * 6  Yellow  (warn)
     * 7  White   (trace)
     */
    inline void setLineColor(int color) 
    {
        SetConsoleTextAttribute(con_handle, color);
    }

    inline void restoreColor() 
    {
        SetConsoleTextAttribute(con_handle, 15);
    }

    // -- TRACE --
    template<typename... Args>
    inline void trace(Args... args) 
    {
        setLineColor(7);
        std::cout << getTimeAsString() << " TRACE   :";
        my_cout(args...);
        std::cout << std::endl;
        restoreColor();
    }

    // -- INFO --
    template<typename... Args>
    inline void info(Args... args) 
    {
        setLineColor(2);
        std::cout << getTimeAsString() << " INFO    :";
        my_cout(args...);
        std::cout << std::endl;
        restoreColor();
    }

    // -- WARNING --
    template<typename... Args>
    inline void warn(Args... args) 
    {
        setLineColor(6);
        std::cout << getTimeAsString() << " WARNING :";
        my_cout(args...);
        std::cout << std::endl;
        restoreColor();
    }

    // -- CRITICAL --
    template<typename... Args>
    inline void critical(Args... args)
    {
        setLineColor(4);
        std::cout << getTimeAsString() << " CRITICAL:";
        my_cout(args...);
        std::cout << std::endl;
        restoreColor();
    }

    #else

    // -- TRACE --
    template<typename... Args>
    inline void trace(Args... args) 
    {
        std::cout << getTimeAsString() << " TRACE   :";
        my_cout(args...);
        std::cout << std::endl;
    }

    // -- INFO --
    template<typename... Args>
    inline void info(Args... args) 
    {
        std::cout << getTimeAsString() << " INFO    :";
        my_cout(args...);
        std::cout << std::endl;
    }

    // -- WARNING --
    template<typename... Args>
    inline void warn(Args... args) 
    {
        std::cout << getTimeAsString() << " WARNING :";
        my_cout(args...);
        std::cout << std::endl;
    }

    // -- CRITICAL --
    template<typename... Args>
    inline void critical(Args... args) 
    {
        std::cout << getTimeAsString() << " CRITICAL:";
        my_cout(args...);
        std::cout << std::endl;
    }

    #endif

    
    
} // namespace Log