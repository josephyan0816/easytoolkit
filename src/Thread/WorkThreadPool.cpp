//
// Created by Joseph Yan on 2023/1/19.
//

#include "WorkThreadPool.h"

namespace toolkit {
    static size_t s_pool_size = 0;
    static bool s_enable_cpu_affinity = true;

    INSTANCE_IMP(WorkThreadPool)

    EventPoller::Ptr WorkThreadPool::getFirstPoller() {
        return std::dynamic_pointer_cast<EventPoller>(_threads.front());
    }

    EventPoller::Ptr WorkThreadPool::getPoller() {
        return std::dynamic_pointer_cast<EventPoller>(getExecutor());
    }

    WorkThreadPool::WorkThreadPool() {
        //最低优先级
        addPoller("work poller", s_pool_size, ThreadPool::PRIORITY_LOWEST, false, s_enable_cpu_affinity);
    }

    void WorkThreadPool::setPoolSize(size_t size) {
        s_pool_size = size;
    }

    void WorkThreadPool::enableCpuAffinity(bool enable) {
        s_enable_cpu_affinity = enable;
    }
}

