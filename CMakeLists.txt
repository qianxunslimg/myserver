cmake_minimum_required(VERSION 3.23)
project(myserver)

set(CMAKE_CXX_STANDARD 14)

#add_executable(myserver main.cpp)

aux_source_directory(. SOURCES) # 搜索当前目录下的所有.cpp文件

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++14 -pthread")

include_directories("/usr/include/mysql")


add_executable(myserver ${SOURCES})

target_link_libraries(myserver libmysqlclient.so)