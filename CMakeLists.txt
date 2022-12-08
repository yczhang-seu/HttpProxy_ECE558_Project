cmake_minimum_required(VERSION 3.23)
project(HttpProxy_ECE558_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(docker-deploy/src)

add_executable(HttpProxy_ECE558_Project
        docker-deploy/src/cache.cpp
        docker-deploy/src/cache.hpp
        docker-deploy/src/LFUCache.cpp
        docker-deploy/src/LFUCache.hpp
        docker-deploy/src/log.cpp
        docker-deploy/src/log.hpp
        docker-deploy/src/LRUCache.cpp
        docker-deploy/src/LRUCache.hpp
        docker-deploy/src/main.cpp
        docker-deploy/src/MyException.h
        docker-deploy/src/proxy.cpp
        docker-deploy/src/proxy.hpp
        docker-deploy/src/request.cpp
        docker-deploy/src/request.hpp
        docker-deploy/src/response.cpp
        docker-deploy/src/response.hpp
        docker-deploy/src/server.cpp
        docker-deploy/src/server.h
        docker-deploy/src/thread_info.h)
