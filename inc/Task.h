//
// Created by 18156 on 2023/10/22.
//

#ifndef THREADPOLL_TASK_H
#define THREADPOLL_TASK_H
#include <any>
#include <iostream>
#include <future>
namespace threadpoll {
    class Task {
    public:
        Task() = default;
        virtual std::any run() = 0;
        template <typename T>
        T getResult(){
            std::any any = promise.get_future().get();
            try
            {
                return std::any_cast<T>(any);
            }
            catch (const std::bad_any_cast &e)
            {
                std::cout << "bad_any_cast" << std::endl;
            }
        }

        void setValue(const std::any& value);

    private:
        std::promise<std::any> promise;
    };
}// threadpoll

#endif //THREADPOLL_TASK_H
