//
// Created by Joseph Yan on 2023/1/8.
//

#ifndef EASYTOOLKIT_ONCETOKEN_H
#define EASYTOOLKIT_ONCETOKEN_H

#include <functional>
#include <type_traits>

namespace toolkit{
    class onceToken{
    public:
        using task=std::function<void(void)>;


        template<typename FUNC>
        onceToken(const FUNC &onConstructed,task onDestructed= nullptr){
            onConstructed();
            _onDestructed=std::move(onDestructed);
        }
        onceToken(std::nullptr_t,task onDestructed= nullptr){
            _onDestructed=std::move(onDestructed);
        }

        ~onceToken(){
            if(_onDestructed){
                _onDestructed();
            }
        }


    private:
        onceToken()=delete;
        onceToken(const onceToken &)=delete;
        onceToken(onceToken &&)=delete;
        onceToken &operator=(const onceToken &)=delete;
        onceToken &operator=(onceToken &&)=delete;
    private:
        task _onDestructed;
    };
}




#endif //EASYTOOLKIT_ONCETOKEN_H
