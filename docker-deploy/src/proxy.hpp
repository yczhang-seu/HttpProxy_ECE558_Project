#include "cache.hpp"
#include "LRUCache.hpp"
#include "LFUCache.hpp"
#include "log.hpp"
#include "server.h"
#include "MyException.h"
#include "thread_info.h"
#include <string>
#include <pthread.h>

#define PORT "12345"

using namespace std;

string fetchGetResponse(Request &request, Cache *cache, int thread_id);
void *process_request(void * information);
void handleGET(int browser_fd, int thread_id, Cache *cache, Request &p);
void handleCONNECT(int browser_fd, int thread_id, const string &hostname, const string &port);
void handlePOST(int browser_fd, int thread_id, Request &p, const string &hostname, const string &port);
void checkRequestValidation(int browser_fd, const string &ip_addr, int thread_id, Request &p);

