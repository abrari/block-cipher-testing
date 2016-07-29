cmake_minimum_required(VERSION 3.5)
project(block_cipher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c num_utils.h num_utils.c sbox_utils.h sbox_utils.c)
add_executable(block_cipher ${SOURCE_FILES})
target_link_libraries(block_cipher m)

set(AWD_SOURCE_FILES awd_main.c awd.h awd.c aes/aes.h aes/aes.c)
add_executable(awd ${AWD_SOURCE_FILES})
