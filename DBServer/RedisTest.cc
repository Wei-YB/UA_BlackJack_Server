#include <lib_acl.hpp>

#include <memory>
#include <string>
#include <iostream>

#include "RedisService.h"

using ua_black_jack_server::DataBaseServer::RedisService;
using std::cout;
using std::endl;

constexpr int defaultScore = 1000;

RedisService* service = nullptr;

int64_t newPlayer(const char* nickname, const char* password) {
    auto newId = service->NextUid();
    service->setPassword(newId, password);
    service->SetNickname(newId, nickname);
    service->SetUid(nickname, newId);
    service->SetScore(newId, defaultScore);
    service->UpdateRank(newId, defaultScore);
    return newId;
}


void printInfo(const char* nickname) {
    if(!service->NameExists(nickname)) {
        cout << "player not exist" << endl;
    }
    auto uidStr = service->GetUid(nickname);
    cout << "UID's size is " << uidStr.size() << endl;
    // cout << "play " << nickname << " info:" << endl;
    // cout << "UID: " <<  << endl;
}

int main() {
    acl::redis_client client("127.0.0.1:6379");
    service = new RedisService(client);


    delete service;
    client.close();
}