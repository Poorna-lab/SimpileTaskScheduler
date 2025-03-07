#include "threadPool.hpp"

namespace tp{
    ThreadPool::ThreadPool(size_t numThreads) : stop(false){
        for(size_t i = 0; i < numThreads; i++){
            workers.emplace_back([this]{
                for(;;){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this]{return stop || !tasks.empty();});
                        if(stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

}