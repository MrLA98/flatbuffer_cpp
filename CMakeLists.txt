cmake_minimum_required(VERSION 3.5)

project(fbs-demo)

set(CMAKE_CXX_STANDARD 11)

# 添加 Flatbuffers 头文件路径
include_directories(/usr/include gen)

# 添加 Flatbuffers 库文件路径
link_directories(/usr/lib)

# 添加可执行文件
file(GLOB CPP_SOURCES src/*.cc)
add_executable(fbs-demo ${CPP_SOURCES})

# 链接 Flatbuffers 库文件
target_link_libraries(fbs-demo flatbuffers)