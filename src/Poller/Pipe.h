//
// Created by Joseph Yan on 2023/1/20.
//

#ifndef EASYTOOLKIT_PIPE_H
#define EASYTOOLKIT_PIPE_H


#include <functional>
#include "PipeWrap.h"
#include "EventPoller.h"


namespace toolkit {
    class Pipe {
    public:
        using onRead = std::function<void(int size, const char *buf)>;

        Pipe(const onRead &cb = nullptr, const EventPoller::Ptr &poller = nullptr);

        ~Pipe();

        void send(const char *send, int size = 0);

    private:
        std::shared_ptr<PipeWarp> _pipe;
        EventPoller::Ptr _poller;
    };
}


#endif //EASYTOOLKIT_PIPE_H
