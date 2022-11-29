#ifndef __CACHE_H__
#define __CACHE_H__

#include "response.hpp"
#include <mutex>

class Cache {
protected:
    int capacity;
    int size;
    std::mutex cache_mutex;
public:
    Cache(int _capacity): capacity(_capacity),size(0) {}
    virtual ~Cache() {}
    virtual string get(const string& key) = 0;
    virtual void put(const string& key, const string& value) = 0;
    bool storeResponse(string uri,Response rsp,int id);
    string revalidate(Request request, Response response, int socket,int id);
    string checkIfNoneMatch(Request request,Response response,int socket,string etag,int id);
    string checkIfModifiedSince(Request request,Response response,int socket,string lastModify,int id);
    string reSendRequest(Request request, int socket,int id);
    string checkValidate(Request request,Response response,int socket, string type,string content,int id);
};

#endif
