//
// Created by 18156 on 2023/10/22.
//

#include "ThreadPoll.h"

namespace threadpoll {
    ThreadPoll::ThreadPoll(unsigned int taskMaxSize,int timeout) :isRunning(false),taskQueMaxSize(taskMaxSize)
            ,timeout(timeout),threadCurrSize(0){
        taskQue = std::make_shared<TaskQue>(taskQueMaxSize);
        size_t size = std::thread::hardware_concurrency();
        threadMinSize = size;
        threadMaxSize = size*2;
    }



    ThreadPoll::~ThreadPoll(){
        isRunning = false;
        std::unique_lock<std::mutex> lock(pollMutex);
        exit.wait(lock,[&](){
            taskQue->getNotEmpty().notify_one();
            return threads.empty();
        });
    }

    void ThreadPoll::start() {
        if (!isRunning){
            isRunning = true;
            for (int i = 0; i < threadMinSize; ++i) {
                addThreadToPoll();
            }
        }
    }

    void ThreadPoll::setThreadMaxSize(unsigned int size) {
        threadMaxSize = size;
    }

    void ThreadPoll::setThreadMinSize(unsigned int size) {
        threadMinSize = size;
    }

    bool ThreadPoll::submitTask(std::shared_ptr<Task> task) {
        bool s = taskQue->submitTask(std::move(task));
        /*如果提交任务阻塞说明线程池处理速度可能不满足当前需求 增加线程数*/
        if (!s && threadCurrSize < threadMaxSize){
            addThreadToPoll();
        }
        return s;
    }

    void ThreadPoll::addThreadToPoll() {
        std::lock_guard<std::mutex> guard(pollMutex);
        std::unique_ptr<Thread> ptr;
        ptr = std::make_unique<Thread>(taskQue, isRunning, timeout,
                                       std::bind(&ThreadPoll::recycleThread, this, std::placeholders::_1),
                                       [&]() { return threadCurrSize > threadMinSize; });
        ptr->start();
        threads.insert({ptr->getId(),std::move(ptr)});
        threadCurrSize++;
    }

    void ThreadPoll::recycleThread(std::thread::id id) {
        std::lock_guard<std::mutex> guard(pollMutex);
        std::cout << "thread " << std::this_thread::get_id() <<"exit\n";
        threads.erase(threads.find(id));
        threadCurrSize--;
        if (!isRunning){
            exit.notify_one();
        }
    }

} // threadpoll