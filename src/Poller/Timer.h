//
// Created by Joseph Yan on 2023/1/20.
//

#ifndef EASYTOOLKIT_TIMER_H
#define EASYTOOLKIT_TIMER_H


#include <functional>
#include "EventPoller.h"

namespace toolkit{
    class Timer{
    public:
        using Ptr = std::shared_ptr<Timer>;

        /**
         * 构造定时器
         * @param second 定时器重复秒数
         * @param cb 定时器任务，返回true表示重复下次任务，否则不重复，如果任务中抛异常，则默认重复下次任务
         * @param poller EventPoller对象，可以为nullptr
         */
         Timer(float second,const std::function<bool()> &cb,const EventPoller::Ptr &poller);
         ~Timer();

    private:
        std::weak_ptr<EventPoller::DelayTask> _tag;
        //定时器保持EventPoller的强引用
        EventPoller::Ptr _poller;
    };
}



#endif //EASYTOOLKIT_TIMER_H
