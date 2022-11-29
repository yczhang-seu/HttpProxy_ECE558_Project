#ifndef __LFUCACHE_H__
#define __LFUCACHE_H__

#include "cache.hpp"
#include <unordered_map>
#include <list>
#include <utility>

using std::unordered_map;
using std::list;

class LFUCache : public Cache {
private:
    class Node {
    public:
        string _key;
        string _value;
        int _freq;
        Node(const string& key, const string& value, int freq): _key(key), _value(value), _freq(freq) {}
    };
    unordered_map<int, list<Node>> freqs;
    unordered_map<string, list<Node>::iterator> keys;
    int _capacity;
    int minFreq;
public:
    LFUCache(int capacity): Cache(capacity), minFreq(0) {}
    virtual ~LFUCache() {}
    virtual string get(const string& key);
    virtual void put(const string& key, const string& value);
};

#endif