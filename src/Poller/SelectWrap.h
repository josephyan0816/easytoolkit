//
// Created by Joseph Yan on 2023/1/20.
//

#ifndef EASYTOOLKIT_SELECTWRAP_H
#define EASYTOOLKIT_SELECTWRAP_H

#include "Util/util.h"

namespace toolkit{
    class FdSet{
    public:
        FdSet();
        ~FdSet();
        void fdZero();
        void fdSet(int fd);
        void fdClr(int fd);
        bool isSet(int fd);
        void *_ptr;
    };

    int zl_select(int cnt,FdSet *read,FdSet *write,FdSet *err,struct timeval *tv);
}


#endif //EASYTOOLKIT_SELECTWRAP_H
