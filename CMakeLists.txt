cmake_minimum_required(VERSION 3.8)

project(BIGINT)

set(CMAKE_CXX_STANDARD 17)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
               big_integer_testing.cpp

               big_integer.h
               big_integer.cpp
               arithmetics.cpp
               auxiliary.cpp
               bitwise.cpp
               comparisons.cpp

               vector.h
               vector.cpp

               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")
endif()

target_link_libraries(big_integer_testing -lpthread)
