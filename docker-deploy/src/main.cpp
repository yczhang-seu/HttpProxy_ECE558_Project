#include "proxy.hpp"

int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Usage: ./main [LRU|LFU]" << endl;
        return -1;
    }
    Cache* cache;
    string arg(argv[1]);
    if (arg == "LRU") {
        cache = new LRUCache(20);
    }
    else if (arg == "LFU") {
        cache = new LFUCache(20);
    }
    else {
        cerr << "Invalid argument!" << endl;
        return -1;
    }
    int thread_id = 0;
    tryOpenFile("./new.log");
    while(1){
        try{
            Server server(PORT);
            string ip_addr;
            int browser_fd = server.accept_connection(&ip_addr);
            thread_id++;
            Thread_info * info = new Thread_info();
            info->browser_fd = browser_fd;
            info->ip_addr = ip_addr;
            info->thread_id = thread_id;
            info->cache = cache;
            pthread_t thread;
            pthread_create(&thread, NULL, process_request, info);
        }catch(std::exception e){
            string log_msg = generateLogMsg(thread_id, e.what());
            writeToLog(log_msg);
            cerr<<thread_id<<": "<<e.what()<<endl;
        }
    }
    delete cache;
    return 0;
}