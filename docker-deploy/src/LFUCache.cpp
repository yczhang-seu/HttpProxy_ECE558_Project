#include "LFUCache.hpp"

string LFUCache::get(const string& key) {
    std::lock_guard<std::mutex> lck(cache_mutex);
    if (!keys.count(key)) {
        return "";
    }
    list<Node>::iterator it = keys[key];
    string value = it->_value;
    int freq = it->_freq;
    freqs[freq].erase(it);
    if (freqs[freq].size() == 0) {
        freqs.erase(freq);
        if (freq == minFreq) {
            minFreq++;
        }
    }
    freq++;
    freqs[freq].push_front(Node(key, value, freq));
    keys[key] = freqs[freq].begin();
    return value;
}

void LFUCache::put(const string& key, const string& value) {
    std::lock_guard<std::mutex> lck(cache_mutex);
    if (_capacity == 0) {
        return;
    }
    if (keys.count(key)) {
        list<Node>::iterator it = keys[key];
        int freq = it->_freq;
        freqs[freq].erase(it);
        if (freqs[freq].size() == 0) {
            freqs.erase(freq);
            if (freq == minFreq) {
                minFreq++;
            }
        }
        freq++;
        freqs[freq].push_front(Node(key, value, freq));
        keys[key] = freqs[freq].begin();
    }
    else {
        if (keys.size() < _capacity) {
            freqs[1].push_front(Node(key, value, 1));
            keys[key] = freqs[1].begin();
            minFreq = 1;
        }
        else {
            Node del = freqs[minFreq].back();
            keys.erase(del._key);
            freqs[minFreq].pop_back();
            freqs[1].push_front(Node(key, value, 1));
            keys[key] = freqs[1].begin();
            minFreq = 1;
        }
    }
}