#include "ThreadPoll.h"
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


    vector<shared_ptr<MyTask>> v;
    for (int i = 0; i < 1000; ++i) {
        v.emplace_back(make_shared<MyTask>(i,i+100));
    }
    threadpoll::ThreadPoll poll(100,10);
    poll.start();

    for (const auto &item: v){
       bool s = poll.submitTask(item) ;
        if (!s){
            poll.submitTask(item) ;
        }
    }

    for (const auto &item: v){
        unsigned long res;
        res = item->getResult<unsigned long>();
        cout << res << endl;
    }


    return 0;
}