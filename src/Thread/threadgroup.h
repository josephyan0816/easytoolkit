//
// Created by Joseph Yan on 2023/1/19.
//

#ifndef EASYTOOLKIT_THREADGROUP_H
#define EASYTOOLKIT_THREADGROUP_H
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <memory>

namespace toolkit{
    class thread_group{
    private:
        thread_group(thread_group const &);
        thread_group &operator=(thread_group const &);
    public:
        thread_group(){}
        ~thread_group(){
            _threads.clear();
        }

        bool is_this_thread_in(){
            auto thread_id=std::this_thread::get_id();
            if(_thread_id==thread_id){
                return true;
            }
            return _threads.find(thread_id)!=_threads.end();
        }
        bool is_thread_in(std::thread *thrd){
            if(!thrd){
                return false;
            }
            auto it=_threads.find(thrd->get_id());
            return it!=_threads.end();
        }

        template<typename F>
        std::thread *create_thread(F &&threadfunc){
            auto thread_new=std::make_shared<std::thread>(threadfunc);
            _thread_id=thread_new->get_id();
            _threads[_thread_id]=thread_new;
            return thread_new.get();
        }

        void remove_thread(std::thread *thrd){
           auto it= _threads.find(thrd->get_id());
            if(it!=_threads.end()){
                _threads.erase(it);
            }
        }

        void join_all(){
            if(is_this_thread_in()){
                throw std::runtime_error("Trying joining itself in thread_group");
            }

            for(auto& it:_threads){
                if(it.second->joinable()){
                    it.second->join();
                }
            }

            _threads.clear();
        }




        size_t size(){
            return _threads.size();
        }


    private:
        std::thread::id _thread_id;
        std::unordered_map<std::thread::id,std::shared_ptr<std::thread>> _threads;
    };
}


#endif //EASYTOOLKIT_THREADGROUP_H
