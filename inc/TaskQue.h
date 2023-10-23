//
// Created by 18156 on 2023/10/22.
//

#ifndef THREADPOLL_TASKQUE_H
#define THREADPOLL_TASKQUE_H
#include <memory>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <queue>
#include "Task.h"

namespace threadpoll {

    class TaskQue {
    public:
        explicit TaskQue(unsigned int maxsize);
        ~TaskQue() = default;
        std::queue<std::shared_ptr<Task>> &getQue();
        // 获取任务队列最大任务数
        const std::atomic_uint &getMaxSize();
        // 设置任务队列最大任务数
        void setMaxSize(unsigned int size);
        // 向任务队列提交任务,提交成功返回ture
        bool submitTask(std::shared_ptr<Task> ptr);
        // 获取互斥锁
        std::mutex &getMutex();
        std::condition_variable &getNotFull();
        std::condition_variable &getNotEmpty();

    private:
        std::queue<std::shared_ptr<threadpoll::Task>> que;
        std::condition_variable notFull;
        std::condition_variable notEmpty;
        std::mutex mutex;

        // 任务队列最大任务数
        std::atomic_uint queMaxSize;
    };
}

#endif //THREADPOLL_TASKQUE_H
