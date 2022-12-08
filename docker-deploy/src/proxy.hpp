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
