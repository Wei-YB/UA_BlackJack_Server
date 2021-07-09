#pragma once
#include "RequestParser.h"

namespace ua_blackjack::data_base_server{

class ThreadPool{
     

    void Insert(CallData* data){
        // insert call data into one blockList
    }

     void threadFunc(){
         RequestParser parser_;
         while(true){
             // read call data from the blocklist
             // parser call data
             // call responder finish
         }
     }

private:
    RequestParser parser_;

};
}