//
// Created by 18156 on 2023/10/22.
//

#include "TaskQue.h"

namespace threadpoll {
    TaskQue::TaskQue(unsigned int maxsize) : queMaxSize(maxsize)
    {
    }

    const std::atomic_uint &TaskQue::getMaxSize() {
        return queMaxSize;
    }

    std::queue<std::shared_ptr<Task>> &TaskQue::getQue() {
        return que;
    }

    bool TaskQue::submitTask(std::shared_ptr<Task> ptr) {
        std::unique_lock<std::mutex> lock(mutex);
        bool success = notFull.wait_for(lock,std::chrono::seconds(1),[&](){
            return que.size() < queMaxSize;
        });
        if (success){
            que.push(std::move(ptr));
            notEmpty.notify_one();
        }
        return success;
    }

    std::mutex &TaskQue::getMutex() {
        return mutex;
    }

    std::condition_variable &TaskQue::getNotFull() {
        return notFull;
    }

    void TaskQue::setMaxSize(unsigned int size) {
        queMaxSize = size;
    }

    std::condition_variable &TaskQue::getNotEmpty() {
        return notEmpty;
    }



} // threadpoll