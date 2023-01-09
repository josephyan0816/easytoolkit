//
// Created by Joseph Yan on 2023/1/5.
//


#include <sys/stat.h>
#include <cstdarg>
#include <iostream>
#include "logger.h"
#include "File.h"
#include "NoticeCenter.h"

#if defined(_WIN32)
#include "strptime_win.h"
#endif
#ifdef ANDROID
#include <android/log.h>
#endif //ANDROID

#if defined(__MACH__) || ((defined(__linux) || defined(__linux__)) && !defined(ANDROID))

#include <sys/syslog.h>

#endif

using namespace std;

namespace toolkit {
#ifdef _WIN32
#define CLEAR_COLOR 7
    static const WORD LOG_CONST_TABLE[][3] = {
            {0x97, 0x09 , 'T'},//蓝底灰字，黑底蓝字，window console默认黑底
            {0xA7, 0x0A , 'D'},//绿底灰字，黑底绿字
            {0xB7, 0x0B , 'I'},//天蓝底灰字，黑底天蓝字
            {0xE7, 0x0E , 'W'},//黄底灰字，黑底黄字
            {0xC7, 0x0C , 'E'} };//红底灰字，黑底红字

    bool SetConsoleColor(WORD Color)
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle == 0)
            return false;

        BOOL ret = SetConsoleTextAttribute(handle, Color);
        return(ret == TRUE);
    }
#else
#define CLEAR_COLOR "\033[0m"
    static const char *LOG_CONST_TABLE[][3] = {
            {"\033[44;37m", "\033[34m", "T"},
            {"\033[42;37m", "\033[32m", "D"},
            {"\033[46;37m", "\033[36m", "I"},
            {"\033[43;37m", "\033[33m", "W"},
            {"\033[41;37m", "\033[31m", "E"}};
#endif

    Logger *g_defaultLogger = nullptr;

    Logger &getLogger() {
        if (!g_defaultLogger) {
            g_defaultLogger = &Logger::Instance();
        }
        return *g_defaultLogger;
    }

    void setLogger(Logger *logger) {
        g_defaultLogger = logger;
    }


///////////////////Logger///////////////////

    INSTANCE_IMP(Logger, exeName())

    Logger::Logger(const string &loggerName) {
        _logger_name = loggerName;
        _last_log = std::make_shared<LogContext>();
    }

    Logger::~Logger() {
        _writer.reset();
        {
            LogContextCapture(*this, LInfo, __FILE__, __FUNCTION__, __LINE__);
        }
        _channels.clear();
    }

    void Logger::add(const std::shared_ptr<LogChannel> &channel) {
        _channels[channel->name()] = channel;
    }

    void Logger::del(const std::string &name) {
        _channels.erase(name);
    }

    std::shared_ptr<LogChannel> Logger::get(const std::string &name) {
        auto it = _channels.find(name);
        if (it == _channels.end()) {
            return nullptr;
        }
        return it->second;
    }

    void Logger::setWriter(const std::shared_ptr<LogWriter> &writer) {
        _writer = writer;
    }

    void Logger::write(const LogContextPtr &ctx) {
        if (_writer) {
            _writer->write(ctx, *this);
        } else {
            writeChannels(ctx);
        }
    }

    void Logger::setLevel(LogLevel level) {
        for (auto &chn: _channels) {
            chn.second->setLevel(level);
        }
    }

    void Logger::writeChannels_l(const LogContextPtr &ctx) {
        for (auto &chn: _channels) {
            chn.second->write(*this, ctx);
        }
        _last_log = ctx;
        _last_log->_repeat = 0;
    }

    //返回毫秒
    static int64_t timevalDiff(struct timeval &a, struct timeval &b) {
        return (1000 * (b.tv_sec - a.tv_sec)) + ((b.tv_usec - a.tv_usec) / 1000);
    }

    void Logger::writeChannels(const LogContextPtr &ctx) {
        if (ctx->_line == _last_log->_line && ctx->_file == _last_log->_file && ctx->str() == _last_log->str()) {
            //重复的日志每隔500ms打印一次，过滤频繁的重复日志
            ++_last_log->_repeat;
            if (timevalDiff(_last_log->_tv, ctx->_tv) > 500) {
                ctx->_repeat = _last_log->_repeat;
                writeChannels_l(ctx);
            }
            return;
        }

        if (_last_log->_repeat) {
            writeChannels_l(_last_log);
        }
        writeChannels_l(ctx);
    }

    const string &Logger::getName() const {
        return _logger_name;
    }

    ///////////////////LogContext///////////////////
    static inline const char *getFileName(const char *file) {
        auto pos = strchr(file, '/');
#ifdef _WIN32
        if(!pos){
        pos = strrchr(file, '\\');
    }
#endif
        return pos ? pos + 1 : file;
    }


    static inline const char *getFunctionName(const char *func) {
#ifndef _WIN32
        return func;
#else
        auto pos = strrchr(func, ':');
    return pos ? pos + 1 : func;
#endif
    }

    LogContext::LogContext(LogLevel level, const char *file, const char *function, int line, const char *module_name,
                           const char *flag) : _level(level), _line(line), _file(getFileName(file)),
                                               _function(getFunctionName(function)),_module_name(module_name),_flag(flag) {
        gettimeofday(&_tv, nullptr);
        _thread_name=getThreadName();
    }


    const string &LogContext::str() {
        if(_got_content){
            return _content;
        }
        _content=ostringstream::str();
        _got_content=true;
        return _content;
    }


}