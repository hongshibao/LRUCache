project(LRUCache)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_BOTH_LIBRARIES})


set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

set(CORE_SRC_DIR ${SRC_DIR}/core)
set(CORE_SRC_LIST
    ${CORE_SRC_DIR}/CacheInterface.h
    ${CORE_SRC_DIR}/LRUCache.h
    ${CORE_SRC_DIR}/LRUCacheShield.h)
add_library(${PROJECT_NAME} SHARED ${CORE_SRC_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(TEST_SRC_DIR ${SRC_DIR}/test)
set(TEST_SRC_LIST
    ${TEST_SRC_DIR}/CacheInterfaceTest.cpp
    ${TEST_SRC_DIR}/LRUCacheShieldTest.cpp
    ${TEST_SRC_DIR}/TestMain.cpp)
add_executable(TestMain ${TEST_SRC_LIST})
target_link_libraries(TestMain ${GTEST_BOTH_LIBRARIES})

