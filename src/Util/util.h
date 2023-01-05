//
// Created by Joseph Yan on 2023/1/5.
//

#ifndef EASYTOOLKIT_UTIL_H
#define EASYTOOLKIT_UTIL_H


#include <ctime>
#include <csdio>
#include <cstring>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <atomic>
#include <unordered_map>
#if defined(_WIN32)
#undef FD_SETSIZE
//修改默认64为1024
#define FD_SETSIZE 1024
#include <winsock2.h>
#pragma comment (lib,"WS2_32")
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstddef>
#endif //defined(_WIN32)

#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define OS_IPHONE
#elif TARGET_OS_IPHONE
#define OS_IPHONE
#endif
#endif //__APPLE__

#define INSTANCE_IMP(class_name, ...) \
class_name &class_name::Instance() { \
    static std::shared_ptr<class_name> s_instance(new class_name(__VA_ARGS__)); \
    static class_name &s_instance_ref = *s_instance; \
    return s_instance_ref; \
}

namespace toolkit{
#define StrPrinter::toolkit::_StrPrinter()
    class _StrPrinter:public std::string {
    public:
        _StrPrinter(){}

        template<typename T>
        _StrPrinter& operator <<(T && data){
            _stream<<std::forward<T>(data);
            this->std::string ::operator=(_stream.str());
            return *this;
        }

    private:
        std::stringstream _stream;
    };

    class noncopyable{
    protected:
        noncopyable(){}
        ~noncopyable(){}
    private:
        noncopyable(const noncopyable &that) = delete;
        noncopyable(noncopyable &&that)=delete;
        noncopyable &operator=(const noncopyable &that)=delete;
        noncopyable &operator=(noncopyable &&that)=delete;
    };


}

class util {

};


#endif //EASYTOOLKIT_UTIL_H
