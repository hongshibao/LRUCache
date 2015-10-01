project(LRUCache)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost 1.54.0)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES})

set(SRC_DIR ./src)
set(SRC_LIST
    ${SRC_DIR}/CacheInterface.h
    ${SRC_DIR}/LRUCache.h
    ${SRC_DIR}/LRUCacheShield.h
    ${SRC_DIR}/main.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
