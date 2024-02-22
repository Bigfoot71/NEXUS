#ifndef NEXUS_UTILS_THREAD_SAFE_QUEUE_HPP
#define NEXUS_UTILS_THREAD_SAFE_QUEUE_HPP

#include "../platform/nxPlatform.hpp"

#include <condition_variable>
#include <mutex>
#include <deque>

namespace nexus { namespace utils {

    /**
     * @brief Thread-Safe Queue (TSQueue) is a container designed for multithreaded access.
     *
     * It provides a safe way to manage a queue of items that can be accessed by multiple threads simultaneously.
     *
     * @tparam T The type of elements stored in the queue.
     */
    template<typename T>
    class NEXUS_API TSQueue
    {
      protected:
        std::mutex muxQueue;                    ///< Mutex for protecting access to the queue.
        std::deque<T> deqQueue;                 ///< The underlying deque container to store items.
        std::condition_variable cvBlocking;     ///< Condition variable for blocking on empty queue.
        std::mutex muxBlocking;                 ///< Mutex for blocking condition variable.

      public:
        TSQueue() = default;
        TSQueue(const TSQueue<T>&) = delete;
        virtual ~TSQueue() { clear(); }

        /**
         * @brief Returns the item at the front of the queue without removing it.
         *
         * @return A constant reference to the front item.
         */
        const T& front()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.front();
        }

        /**
         * @brief Returns the item at the back of the queue without removing it.
         *
         * @return A constant reference to the back item.
         */
        const T& back()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.back();
        }

        /**
         * @brief Removes and returns the item from the front of the queue.
         *
         * @return The item removed from the front of the queue.
         */
        T pop_front()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(deqQueue.front());
            deqQueue.pop_front();
            return t;
        }

        /**
         * @brief Removes and returns the item from the back of the queue.
         *
         * @return The item removed from the back of the queue.
         */
        T pop_back()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(deqQueue.back());
            deqQueue.pop_back();
            return t;
        }

        /**
         * @brief Adds an item to the back of the queue.
         *
         * @param item The item to be added to the queue.
         */
        void push_back(const T& item)
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_back(std::move(item));

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        /**
         * @brief Adds an item to the front of the queue.
         *
         * @param item The item to be added to the queue.
         */
        void push_front(const T& item)
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_front(std::move(item));

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        /**
         * @brief Checks if the queue is empty.
         *
         * @return True if the queue has no items, false otherwise.
         */
        bool empty()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.empty();
        }

        /**
         * @brief Returns the number of items in the queue.
         *
         * @return The number of items in the queue.
         */
        size_t count()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.size();
        }

        /**
         * @brief Clears the entire queue.
         */
        void clear()
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.clear();
        }

        /**
         * @brief Blocks the calling thread until the queue is not empty.
         */
        void wait()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> ul(muxBlocking);
                cvBlocking.wait(ul);
            }
        }
    };

}}

#endif //NEXUS_UTILS_THREAD_SAFE_QUEUE_HPP
