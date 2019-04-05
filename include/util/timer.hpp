#pragma once

#include <chrono>
#include <ostream>

#include "util/log.hpp"

// Simple timer struct that can be initialized in beginning of function.
// Will call destructor when goes out of scope.
// Logs total lifetime of timer.
struct Timer {

    std::chrono::high_resolution_clock::time_point start, lastTime;
    std::string operation;

    Timer(std::string const& oper = "") 
    {
        operation = oper;
        start = std::chrono::high_resolution_clock::now();
        lastTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0f;
        LOG_TRACE("--TIMER--", (operation != "") ? operation : "Timing took:", dur, "ms");
    }

    std::chrono::high_resolution_clock::time_point getTime()
    {
        return std::chrono::high_resolution_clock::now();
    }

    std::chrono::high_resolution_clock::time_point getLastTime()
    {
        return lastTime;
    }
    
    // Returns elapsed time as seconds and updates lastTime
    float getElapsedTimeReset()
    {
        auto t = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t - lastTime).count() / 1000000.0f;
        lastTime = t;
        return elapsed;
    }

    // Returns elapsed time as microseconds
    float getElapsedTime()
    {
        auto t = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(t - lastTime).count() / 1000000.0f;
    }
};