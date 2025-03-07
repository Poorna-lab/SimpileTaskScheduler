#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#include <stdexcept>

namespace tp
{

    class ThreadPool
    {
    public:
        // Constructor: Initializes the thread pool with a given number of worker threads.
        explicit ThreadPool(size_t numThreads);
        // enqueue() function:
        // - Takes a function `f` and its arguments `args`
        // - Wraps the function in a `std::packaged_task` to support return values
        // - Pushes the task into the queue and notifies a worker thread
        template <class F, class... Args>
        auto enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result_t<F, Args...>>;
        // Destructor: Waits for all threads to finish and properly cleans up resources.
        ~ThreadPool();
        // Delete copy constructor and assignment operator to prevent copying.
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;

    private:
        // Worker threads that will execute the submitted tasks.
        std::vector<std::thread> workers;
        // Task queue that holds submitted tasks.
        std::queue<std::function<void()>> tasks;
        // Mutex to synchronize access to the task queue.
        std::mutex queueMutex;
        // Condition variable to notify worker threads when new tasks are available.
        std::condition_variable condition;
        // Flag to indicate whether the thread pool is stopping.
        bool stop;
    };
}
#endif
