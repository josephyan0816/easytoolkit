//
// Created by Joseph Yan on 2023/1/20.
//

#ifndef EASYTOOLKIT_BUFFER_H
#define EASYTOOLKIT_BUFFER_H

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include <functional>
#include "Util/util.h"
#include "Util/ResourcePool.h"

namespace toolkit{
    template<typename T> struct is_pointer:public std::false_type {};
    template<typename T> struct is_pointer<std::shared_ptr<T>>:public std::true_type {};
    template<typename T> struct is_pointer<std::shared_ptr<T const>>:public std::true_type {};
    template<typename T> struct is_pointer<T*>:public std::true_type {};
    template<typename T> struct is_pointer<const T*>:public std::true_type {};

    //缓存基类
    class Buffer:public noncopyable{

    };

    template<typename C>
    class BufferOffset:public Buffer{

    };

    using BufferString=BufferOffset<std::string>;

    //指针式缓存对象
    class BufferRaw:public Buffer{
    public:
        using Ptr=std::shared_ptr<BufferRaw>;
    };

    class BufferLikeString:public Buffer{

    };


}


#endif //EASYTOOLKIT_BUFFER_H
