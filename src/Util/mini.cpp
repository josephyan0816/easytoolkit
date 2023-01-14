//
// Created by Joseph Yan on 2023/1/14.
//

#include "mini.h"

using namespace std;
namespace toolkit{
    template<>
    mINI_basic<string,variant> &mINI_basic<string,variant>::Instance(){
        static mINI_basic<string, variant> instance;
        return instance;
    }

    template<>
    bool variant::as<bool>() const{
        if(empty()|| isdigit(front())){
            //数字开头
            return as_default<bool>();
        }

        if(strToLower(std::string(*this))=="true"){
            return true;
        }
        if(strToLower(std::string(*this))=="false"){
            return false;
        }
        //未识别字符串
        return as_default<bool>();
    }


    template<>
    uint8_t variant::as<uint8_t >() const{
        return 0xFF & as_default<int>();
    }

}