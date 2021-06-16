#include <lib_acl.hpp>
#include <memory>
#include <string>
#include <iostream>

using std::shared_ptr;
using acl::redis_client;
using acl::redis_hash;

using UserId = int64_t;


constexpr int initScore = 1000;
constexpr int initFriends = 0;



shared_ptr<redis_client> client;

// TODO: maybe we can reuse this string
std::string toName(UserId id){
    return std::to_string(id) + ":name";
}

std::string toScore(UserId id){
    return std::to_string(id) + ":score";
}

std::string toPassword(UserId id){
    return std::to_string(id) + ":password";
}

std::string toName(const std::string& nickName){
    return nickName + ":name";
}

bool keyExists(const std::string& key){
    acl::redis cmd(client.get());
    if(cmd.exists(key.c_str()))
        return true;
    return false;
}

UserId getNewUID(){
    // acl::string id;
    char buffer[64];
    bzero(buffer, sizeof buffer);
    
    int result = 0;

    acl::redis_string redisString(client.get());
    redisString.incr("UID");
    auto ret = redisString.get("UID");
    ret->argv_to_string(buffer, sizeof buffer);
    return strtoull(buffer, nullptr, 10);
}

// sign up new user
UserId signUp(const char* nickName, const char* password){
    
    auto id = getNewUID();
    auto nickNameKey = toName(nickName);
    if(keyExists(nickNameKey)){     // duplicate nickname
        return -1;
    }
    acl::redis cmd(client.get());
    cmd.set(toName(id).c_str(), nickName);
    cmd.set(toPassword(id).c_str(), password);
    cmd.set(toName(nickName).c_str(), std::to_string(id).c_str());
    // TODO: init friendlist, waitingList, score, matchList
    // TODO: update rank
    return id;
}

std::string getName(UserId uid){
    acl::redis_string cmd(client.get());
    auto ret = cmd.get(toName(uid).c_str());

    
}

int main(){
    client = std::make_shared<redis_client>("127.0.0.1:6379");

    for(int i = 0; i < 1000; ++i){
        std::cout << getNewUID() << std:: endl;
    }

}





