cmake_minimum_required(VERSION 3.1)
project(myserver)

set(CMAKE_CXX_STANDARD 14)

# 使用 aux_source_directory 命令搜索 src 目录下的所有 .cpp 文件
aux_source_directory(src SOURCES)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++14 -pthread")

include_directories("/usr/include/mysql" include)


add_executable(myserver ${SOURCES})

target_link_libraries(myserver libmysqlclient.so)
