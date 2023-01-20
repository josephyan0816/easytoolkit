//
// Created by Joseph Yan on 2023/1/19.
//

#ifndef EASYTOOLKIT_WORKTHREADPOOL_H
#define EASYTOOLKIT_WORKTHREADPOOL_H


#include <memory>
#include "Poller/EventPoller.h"

namespace toolkit{
    class WorkThreadPool:public std::enable_shared_from_this<WorkThreadPool>,public TaskExecutorGetterImp{
    public:
        using Ptr = std::shared_ptr<WorkThreadPool>;

        ~WorkThreadPool() override=default;

        /**
         * 获取单例
         */
         static WorkThreadPool &Instance();

         /**
          * 设置EventPoller个数，在WorkThreadPool单例创建前有效
          * 在不调用此方法的情况下，默认创建thread::hardware_concurrency()个EventPoller实例
          * @param size EventPoller个数，如果为0则为thread::hardware_concurrency()
          *
          */

         static void setPoolSize(size_t size=0);

         /**
          * 内部创建线程是否设置cpu亲和性，默认设置cpu亲和性
          */
          static void enableCpuAffinity(bool enable);


          /**
           * 获取第一个实例
           * @return
           */
           EventPoller::Ptr getFirstPoller();

           /**
            * 根据负责情况获取轻负载的实例
            * 如果优先返回当前线程，那么会返回当前线程
            * 返回当前线程的目的是为了提高线程安全性
            */
            EventPoller::Ptr getPoller();



    protected:
        WorkThreadPool();
    };
}


#endif //EASYTOOLKIT_WORKTHREADPOOL_H
