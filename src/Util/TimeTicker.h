//
// Created by Joseph Yan on 2023/1/21.
//

#ifndef EASYTOOLKIT_TIMETICKER_H
#define EASYTOOLKIT_TIMETICKER_H

#include <cassert>
#include "logger.h"


namespace toolkit {
    class Ticker {
    public:
        /**
         * 此对象可以用于代码执行时间统计，也可以用于一般计时
         * @param min_ms 开启码执行时间统计时，如果代码执行耗时超过该参数，则打印警告日志
         * @param ctx 日志上下文捕获，用于捕获当前日志代码所在位置
         * @param print_log 是否打印代码执行时间
         */
        Ticker(uint64_t min_ms = 0,
               LogContextCapture ctx = LogContextCapture(Logger::Instance(), LWarn, __FILE__, "", __LINE__),
               bool print_log = false) : _ctx(std::move(ctx)) {
            if (!print_log) {
                _ctx.clear();
            }
        }

        ~Ticker() {
            uint64_t tm = createdTime();
            if (tm > _min_ms) {
                _ctx << "take time: " << tm << "ms" << ",thread may be overloaded";
            } else {
                _ctx.clear();
            }

        }

        /**
         * 获取上次resetTime后至今的时间，单位毫秒
         */
        uint64_t elapsedTime() const {
            return getCurrentMillisecond() - _begin;
        }

        /**
         * 获取创建至今的时间，单位毫秒
         */
        uint64_t createdTime() const {
            return getCurrentMillisecond() - _created;
        }

        /**
         * 重置计时器
         */
        void resetTime() {
            _begin = getCurrentMillisecond();
        }


    private:
        uint64_t _min_ms;
        uint64_t _begin;
        uint64_t _created;
        LogContextCapture _ctx;
    };
}


#endif //EASYTOOLKIT_TIMETICKER_H
