//
// Created by Joseph Yan on 2023/1/5.
//

#ifndef EASYTOOLKIT_SEMAPHORE_H
#define EASYTOOLKIT_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

namespace toolkit {
    class semaphore {
    public:
        explicit semaphore(size_t initial = 0) {
#if defined(HAVA_SEM)
            sem_init(&_sem,0,initial);
#else
            _count = 0;
#endif
        }

        ~semaphore() {
#if defined(HAVA_SEM)
            sem_destory(&_sem);
#endif
        }

        void post(size_t n = 1) {
#if defined(HAVE_SEM)
            while(n--){
                sem_post(&_sem);
            }
#else
            std::unique_lock <std::recursive_mutex> lock(_mutex);
            _count += n;
            if (n == 1) {
                _condition.notify_one();
            } else {
                _condition.notify_all();
            }
#endif
        }

        void wait() {
#if defined(HAVE_SEM)
            sem_wait(&_sem);
#else
            std::unique_lock <std::recursive_mutex> lock(_mutex);
            while (_count == 0) {
                _condition.wait(lock);
            }
            --_count;
#endif

        }

    private:
#if defined(HAVE_SEM)
        sem_t _sem;
#else
        size_t _count;
        std::recursive_mutex _mutex;
        std::condition_variable_any _condition;
#endif
    };
}


#endif //EASYTOOLKIT_SEMAPHORE_H
