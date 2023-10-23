//
// Created by 18156 on 2023/10/22.
//

#include "Task.h"
namespace threadpoll {
    void Task::setValue(const std::any &value) {
        promise.set_value(value);
    }
} // threadpoll