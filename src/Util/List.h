//
// Created by Joseph Yan on 2023/1/5.
//

#ifndef EASYTOOLKIT_LIST_H
#define EASYTOOLKIT_LIST_H

#include <list>
#include <type_traits>

namespace toolkit {
#if 0
#else

    template<typename T>
    class List : public std::list<T> {
    public:
        template<typename ... ARGS>
        List(ARGS &&...args):std::list<T>(std::forward<ARGS>(args)...) {};

        ~List() = default;

        void append(List<T> &other) {
            if (other.empty()) {
                return;
            }
            this->insert(this->end(), other.begin(), other.end());
            other.clear();
        }

        template<typename FUNC>
        void for_each(FUNC &&func) {
            for (auto &t: *this) {
                func(t);
            }
        }

        template<typename FUNC>
        void for_each(FUNC &&func) const {
            for (auto &t: *this) {
                func(t);
            }
        }

    };
#endif
}


#endif //EASYTOOLKIT_LIST_H
