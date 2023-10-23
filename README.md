# threadpoll_cpp

## Introduce

这是一个基于c++17 所编写的线程池项目，该线程池可动态调节线程池线程数量大小。默认线程数为cpu物理核心数，该线程池支持获取任务的返回值，该返回值可为任意类型

### example

```c++
#include "ThreadPoll.h"
//继承 threadpoll::Task,由用户自定义实现纯虚函数 run() 
//run() 函数是交给线程池所执行的实际任务
class MyTask:public threadpoll::Task{
public:
    int begin;
    int end;
public:
    explicit MyTask(int b,int e):begin(b),end(e){

    }
    virtual std::any run() override{
        unsigned long sum=0;

        for (unsigned int i = begin; i <= end ; ++i) {
            sum+=i;
        }
        return sum;
    }
};
using namespace std;
int main(){

	//定义一个vector来存储准备提交给线程池的任务
    vector<shared_ptr<MyTask>> v;
    for (int i = 0; i < 1000; ++i) {
        v.emplace_back(make_shared<MyTask>(i,i+100));
    }
    /*
    参数1:任务队列最大任务数
    参数2:线程最大等待时间，如果线程等待超时，那么由线程池决定改线程是否回收
    */
    threadpoll::ThreadPoll poll(100,10);
    poll.start();

    for (const auto &item: v){
        //向线程池提交任务，s表示是否提交成功
       bool s = poll.submitTask(item) ;
            poll.submitTask(item) ;
    }

    for (const auto &item: v){
        unsigned long res;
        //获取执行结果
        res = item->getResult<unsigned long>();
        cout << res << endl;
    }
    return 0;
}
```