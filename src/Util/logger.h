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

namespace toolkit {
    class LogContext;

    class LogChannel;

    class LogWriter;

    class Logger;

    using LogContextPtr = std::shared_ptr<LogContext>;

    typedef enum {
        LTrace = 0, LDebug, LInfo, LWarn, LError
    } LogLevel;

    Logger &getLogger();

    void setLogger(Logger *logger);


    class Logger : public std::enable_shared_from_this<Logger>, public noncopyable {
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
        void add(const std::shared_ptr <LogChannel> &channel);

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
        std::shared_ptr <LogChannel> get(const std::string &name);

        /*
         * 设置写log器，非线程安全的
         * @param writer 写log器
         */
        void setWriter(const std::shared_ptr <LogWriter> &writer);

        /*
         * 设置所有日志通道的log等级
         * @param level log等级
         */
        void setLevel(LogLevel level);

        /*
         * 获取logger名
         * @return logger名
         */
        const std::string &getName() const;

        /*
         * 写日志
         * @param ctx 日志信息
         */
        void write(const LogContextPtr &ctx);

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
        std::shared_ptr <LogWriter> _writer;
        std::map <std::string, std::shared_ptr<LogChannel>> _channels;
    };

    /*
     * 日志上下文
     */
    class LogContext : public std::ostringstream {
    public:
        LogContext() = default;

        LogContext(LogLevel level, const char *file, const char *function, int line, const char *module_name,
                   const char *flag);

        ~LogContext() = default;

        LogLevel _level;
        int _line;
        int _repeat = 0;
        std::string _file;
        std::string _function;
        std::string _thread_name;
        std::string _module_name;
        std::string _flag;
        struct timeval _tv;

        const std::string &str();

    private:
        bool _got_content = false;
        std::string _content;
    };

/*
 * 日志上下文捕获器
 */

    class LogContextCapture {
    public:
        using Ptr = std::shared_ptr<LogContextCapture>;

        LogContextCapture(Logger &logger, LogLevel level, const char *file, const char *function, int line,
                          const char *flag = "");

        LogContextCapture(const LogContextCapture &that);

        ~LogContextCapture();

        /**
         * 输入std::endl(回车符)立刻输出日志
         * @param f std::endl(回车符)
         * @return 自身引用
         */
        LogContextCapture &operator<<(std::ostream &(*f)(std::ostream &));

        template<typename T>
        LogContextCapture &operator<<(T &&data) {
            if (!_ctx) {
                return *this;
            }
            (*_ctx) << std::forward<T>(data);
            return *this;
        }

        void clear();

    private:
        LogContextPtr _ctx;
        Logger &_logger;
    };

    /**
     * 写日志器
     */
    class LogWriter:public noncopyable{
    public:
        LogWriter()=default;
        virtual ~LogWriter()=default;
        virtual void write(const LogContextPtr &ctx,Logger &logger)=0;
    };

    class AsyncLogWriter:public LogWriter{
    public:
        AsyncLogWriter();
        ~AsyncLogWriter();
    private:
        void run();
        void flushAll();
        void write(const LogContextPtr &ctx,Logger &logger) override;
    private:
        bool _exit_flag;
        semaphore _sem;
        std::mutex _mutex;
        std::shared_ptr<std::thread> _thread;
        List<std::pair<LogContextPtr,Logger *> > _pending;
    };

}


#endif //EASYTOOLKIT_LOGGER_H
