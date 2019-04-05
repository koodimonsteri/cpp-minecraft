#pragma once

//#include <queue>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <functional>

/*
 * Thread safe queue for communicating chunks between game and loader
 * Uses std::move() operation since we are dealing with unique pointers
 */
// TODO: switch from lock_guard to unique_lock
template<class T>
class SafeQueue 
{
    public:
        SafeQueue(): q(), m() {};
        //~SafeQueue();

        // Push 1 element to queue
        void q_push(T t)
        {
            std::unique_lock<std::mutex> lock(m); // Lock
            q.push_back(std::move(t));                 // Push element to queue
            lock.unlock();
            //c.notify_one();                       // Notify condition_variable
        }

        // Push all elements from vector to queue
        void q_push_all(std::vector<T> data_v)
        {
            std::unique_lock<std::mutex> lock(m);  // Lock
            for(auto t : data_v)
            {
                q.push_back(std::move(t));             // Push all elements to queue
            }
            lock.unlock();
            //c.notify_one();                       // Notify condition_variable
        }

        // Pop queue and return first value
        // Must call q_empty() before calling this function
        T q_pop()
        {
            std::unique_lock<std::mutex> lock(m); // Lock
            T temp = std::move(q.front());        // Get reference to first element in queue
            q.pop_front();
            lock.unlock();
            return temp;                          // Return
        }

        std::vector<T> q_pop_all()
        {
            std::unique_lock<std::mutex> lock(m);
            std::vector<T> temp;
            while(!q.empty())
            {
                temp.push_back(std::move(q.front()));
                q.pop_front();
            }
            lock.unlock();
            return temp;
        }

        // Check if queue is empty
        bool q_empty()
        {
            std::unique_lock<std::mutex> lock(m);  // Lock
            bool temp = q.empty();                // Get return value
            lock.unlock();
            return temp;
        }

        bool q_exists(std::function<bool(const T&)> predicate) const {
            std::unique_lock<std::mutex> lock(m); 
            for (auto &elem : q) {
                if (predicate(elem)) return true;
            }
            lock.unlock();
            return false;
        }

    private:
        std::deque<T>           q;   // Our queue
        mutable std::mutex      m;   // Mutex
        //std::condition_variable c;   // Condition variable
};