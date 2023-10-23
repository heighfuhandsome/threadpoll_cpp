//
// Created by 18156 on 2023/10/22.
//

#include "Thread.h"

#include <utility>

namespace threadpoll {

    Thread::Thread(std::shared_ptr<TaskQue> taskQue, const std::atomic_bool &isRunning, int waitTime,
                   std::function<void(std::thread::id)> exit, std::function<bool(void)> whether):taskQue(std::move(taskQue)),pollIsRunning(isRunning),maxWaitTime(std::chrono::seconds(waitTime))
            ,threadExit(std::move(exit)),whetherRecycle(std::move(whether)){

    }

    void Thread::run() {
        while (true) {
            std::unique_lock<std::mutex> lock(taskQue->getMutex());
            bool timout = taskQue->getNotEmpty().wait_for(lock,maxWaitTime,[&](){
                return !taskQue->getQue().empty();
            });

            /*如果线程池停止运行了*/
            if (!pollIsRunning){
                threadExit(id);
                return;
            }

            if (timout){
                /*未超时*/
                auto ptr = taskQue->getQue().front();
                taskQue->getQue().pop();
                lock.unlock();
                taskQue->getNotFull().notify_one();
                ptr->setValue(ptr->run());
            } else{
                /*超时*/
                if (whetherRecycle()){
                    threadExit(id);
                    return;
                }
            }
        }
    }

    std::thread::id Thread::getId() {
        return id;
    }



    void Thread::start( ) {
        thread = std::make_unique<std::thread>(&Thread::run, this);
        id = thread->get_id();
        thread->detach();
    }



} // threadpoll