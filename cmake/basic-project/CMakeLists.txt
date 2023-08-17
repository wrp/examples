
cmake_minimum_required(VERSION 3.26)
project(hello-world VERSION 1.2.3.4)
add_executable(hello src/hello.c)
include_directories(include)
configure_file(
    ${PROJECT_SOURCE_DIR}/include/config.h.in
    ${PROJECT_SOURCE_DIR}/include/config.h
)
