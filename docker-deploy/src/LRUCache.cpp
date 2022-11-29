#include "LRUCache.hpp"

string LRUCache::get(const string& key) {
    std::lock_guard<std::mutex> lck(cache_mutex);
    if (!_map.count(key)) {
        return "";
    }
    list<pair<string, string>>::iterator it = _map[key];
    string ans = it->second;
    _list.erase(it);
    _list.push_front(make_pair(key, ans));
    _map[key] = _list.begin();
    return ans;
}

void LRUCache::put(const string& key, const string& value) {
    std::lock_guard<std::mutex> lck(cache_mutex);
    if (_map.count(key)) {
        list<pair<string, string>>::iterator it = _map[key];
        _list.erase(it);
        _list.push_front(make_pair(key, value));
    }
    else {
        if (_list.size() < capacity) {
            _list.push_front(make_pair(key, value));
            _map[key] = _list.begin();
        }
        else {
            _map.erase(_list.back().first);
            _list.pop_back();
            _list.push_front(make_pair(key, value));
            _map[key] = _list.begin();
        }
    }
}