//
// Created by Joseph Yan on 2023/1/5.
//

#ifndef EASYTOOLKIT_LOGGER_H
#define EASYTOOLKIT_LOGGER_H
#include <set>
#include <map>
#include <fstream>
#include <thread>
#include <memory>
#include <mutex>
#include "util.h"
#include "List.h"
#include "Thread/semaphore.h"

namespace toolkit{
    class LogContext;
    class LogChannel;
    class LogWriter;
    class Logger;

    using LogContextPtr=std::shared_ptr<LogContext>;

    typedef enum {
        LTrace=0,LDbug,LInfo,LWarn,LError
    } LogLevel;
    Logger& getLogger();
    void setLogger(Logger *logger);


    class Logger:public std::enable_shared_from_this<Logger>,public noncopyable{
    public:
        friend class AsyncLogWriter;
        using Ptr = std::shared_ptr<Logger>;

        /*
         * 获取日志单例
         * @return
         */
        static Logger &Instance();

        explicit Logger(const std::string &loggerName);
        ~Logger();

        /*
         * 添加日志通道，非线程安全的
         *  @param channel log通道
         */
        void add(const std::shared_ptr<LogChannel> &channel);

        /*
         * 删除日志通道，非线程安全的
         * @param name log通道名
         */


    };

}




#endif //EASYTOOLKIT_LOGGER_H
