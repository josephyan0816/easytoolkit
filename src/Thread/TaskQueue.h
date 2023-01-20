//
// Created by Joseph Yan on 2023/1/19.
//

#ifndef EASYTOOLKIT_TASKQUEUE_H
#define EASYTOOLKIT_TASKQUEUE_H

#include <mutex>
#include "Util/List.h"
#include "semaphore.h"

namespace toolkit {

//实现了一个基于函数对象的任务列队，该列队是线程安全的，任务列队任务数由信号量控制
    template<typename T>
    class TaskQueue {
    public:
        //打入任务至队列
        template<typename C>
        void push_task(C &&task_func){
            {
                std::lock_guard<decltype(_mutex)>lock(_mutex);
                _queue.emplace_back(std::forward<C>(task_func));
            }
            _sem.post();
        }

        template<typename C>
        void push_task_first(C &&task_func){
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                _queue.emplace_front(std::forward<C>(task_func));
            }
            _sem.post();
        }

        //清空任务队列
        void push_exit(size_t n){
            _sem.post(n);
        }

        //从队列获取一个任务，由执行线程执行
        bool get_task(T &tsk){
            _sem.wait();
            std::lock_guard<decltype(_mutex)> lock(_mutex);
            if(_queue.empty()){
                return false;
            }
            tsk=std::move(_queue.front());
            _queue.pop_front();
            return true;
        }

        size_t size() const{
            std::lock_guard<decltype(_mutex)> lock(_mutex);
            return _queue.size();
        }



    private:
        List<T> _queue;
        mutable std::mutex _mutex;
        semaphore _sem;
    };
}


#endif //EASYTOOLKIT_TASKQUEUE_H
