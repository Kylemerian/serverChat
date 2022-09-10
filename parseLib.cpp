#include <string>
#include <vector>
#include <SFML/Network.hpp>
#include "database.hpp"
#include "socketAPI.hpp"

enum{
    authCode = 0,
    registerCode = 1,
    messageCode = 2,
    historyCode = 3,
    chatsCode = 4,
    createChatCode = 5,
    errorCode = -1
};

std::string getFirstArg(std::string s){
    std::string res = "";
    for(auto c : s)
        if(std::isspace(c) || c == '\n')
            break;
        else
            res += c;
    return res;
}

std::vector<std::string> getArgs(std::string s) {
    std::vector<std::string> v;
    std::string tmp = "";
    for(int i = 0; i < s.size(); i++){
        if(!isspace(s[i]))
            tmp += s[i];
        else{
            v.push_back(tmp);
            tmp = "";
        }
    }
    return v;
}

int requestIdentify(std::string request){
    std::vector <std::string> requests = {"#auth", "#register",
        "#message", "#history", "#chats", "#createchat"};
    
    std::string cmd = getFirstArg(request);
    for(int i = 0; i < requests.size(); i++){
        if(cmd == requests[i]){
            return i;
        }
    }
    return -1;
}

void requestHandler(std::pair<sf::TcpSocket *, int> * client, std::string request, Database &db){
    int res;
    int reqCode = requestIdentify(request);
    std::string outS;
    std::vector<std::string> args = getArgs(request);
    
    switch (reqCode){
        case authCode: //   #auth login password\n
            res = auth(*(client -> first), db, args[1], args[2]);
            client -> second = res;
        break;
        
        case registerCode: //   #register nickname login password
            registration(*(client -> first), db, args[1], args[2], args[3]);
        break;
        
        case messageCode:
            message(*(client -> first), db, client -> second, args[1], args[2]);
        break;
        
        case historyCode:
            history(*(client -> first), db, args[1], args[2], args[3]);
        break;
        
        case chatsCode:
            chats(*(client -> first), db, client -> second);
        break;
        
        case createChatCode:
            createChat(*(client -> first), db, args);
        break;

        case errorCode:
            return;
    }
}