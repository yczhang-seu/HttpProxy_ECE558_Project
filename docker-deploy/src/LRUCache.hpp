#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include "cache.hpp"
#include <unordered_map>
#include <list>
#include <utility>

using std::unordered_map;
using std::list;
using std::pair;

class LRUCache : public Cache {
private:
    unordered_map<string, list<pair<string, string>>::iterator> _map;
    list<pair<string, string>> _list;
public:
    LRUCache(int capacity): Cache(capacity) {}
    virtual ~LRUCache() {}
    virtual string get(const string& key);
    virtual void put(const string& key, const string& value);
};

#endif