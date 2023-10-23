//
// Created by 18156 on 2023/10/22.
//
#ifndef THREADPOLL_THREADPOLL_H
#define THREADPOLL_THREADPOLL_H
#include "Thread.h"
#include <unordered_map>

namespace threadpoll{
    class ThreadPoll {
    public:
        explicit ThreadPoll(unsigned int taskMaxSize,int timeout);
        ThreadPoll(const ThreadPoll&) = delete;
        ThreadPoll& operator=(const ThreadPoll&) = delete;
        ~ThreadPoll();
        void start();
        void setThreadMaxSize(unsigned int size);
        void setThreadMinSize(unsigned int size);
        bool submitTask(std::shared_ptr<Task> task);
    private:
        /*回收线程*/
        void recycleThread(std::thread::id id);

        /*向线程池添加线程*/
        void addThreadToPoll();
    private:
        /*存储Thread对象的实例*/
        std::unordered_map<std::thread::id,std::unique_ptr<Thread>> threads;
        /*线程池最大线程数*/
        std::atomic_uint threadMaxSize;
        /*线程池最小线程数*/
        std::atomic_uint threadMinSize;
        /*线程池当前线程数*/
        std::atomic_uint threadCurrSize;
       /*线程池互斥锁 保护线程池线程安全*/
       std::mutex pollMutex;
        /*线程最大等待时间*/
        unsigned int timeout;
        /*任务队列*/
        std::shared_ptr<TaskQue> taskQue;
       /*任务队列最大任务数*/
       unsigned int taskQueMaxSize;

        /*线程池是否运行中*/
        std::atomic_bool isRunning;
        std::condition_variable exit;
    };
}



#endif //THREADPOLL_THREADPOLL_H
