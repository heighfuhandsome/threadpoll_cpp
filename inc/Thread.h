//
// Created by 18156 on 2023/10/22.
//

#ifndef THREADPOLL_THREAD_H
#define THREADPOLL_THREAD_H
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include "TaskQue.h"
namespace threadpoll {

    class Thread {
    public:
        Thread(std::shared_ptr<TaskQue> taskQue, const std::atomic_bool &isRunning, int waitTime,
               std::function<void(std::thread::id)> exit, std::function<bool(void)> whether);
        ~Thread() =default;
        void start();
        std::thread::id getId();
    private:
        /*线程实例所执行的函数*/
        void run();

    private:
        /*当前线程的id*/
        std::thread::id id;
        /*当前线程所消费的任务队列*/
        std::shared_ptr<TaskQue> taskQue;
        /*当前线程对象所维护的线程实例*/
        std::unique_ptr<std::thread> thread;
        /*由线程池预先设定的函数判断当该线程超时时是否被回收*/
        std::function<bool(void)> whetherRecycle;
        /*当线程推出由线程池负责回收*/
        std::function<void(std::thread::id)> threadExit;
        /*线程池是否是启动中*/
        const std::atomic_bool &pollIsRunning;
        /*线程最大等待时长*/
        std::chrono::seconds maxWaitTime;
    };

} // threadpoll

#endif //THREADPOLL_THREAD_H
