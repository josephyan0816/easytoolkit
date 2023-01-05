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
        void del(const std::string &name);

        /*
         * 获取日志通道，非线程安全的
         * @param name log通道名
         * @return 线程通道
         */
        std::shared_ptr<LogChannel> get(const std::string &name);

        /*
         * 设置写log器，非线程安全的
         * @param writer 写log器
         */
        void setWriter(const std::shared_ptr<LogWriter> &writer);

        /*
         * 设置所有日志通道的log等级
         * @param level log等级
         */
        void setLevel(LogLevel level);

        /*
         * 获取logger名
         * @return logger名
         */
        const std::string &gewtName() const;

        /*
         * 写日志
         * @param ctx 日志信息
         */
        void write(const LogContextPrt &ctx);
    private:
        /*
         * 写日志到各channel，仅供AsyncLogWriter调用
         * @param ctx日志信息
         * */
       void writeChannels(const LogContextPtr &ctx);
       void writeChannels_l(const LogContextPtr &ctx);
    private:
        LogContextPtr _last_log;
        std::string _logger_name;
        std::shared_ptr<LogWriter> _writer;
        std::map<std::string,std::shared_ptr<LogChannel>> _channels;
    };

}




#endif //EASYTOOLKIT_LOGGER_H
