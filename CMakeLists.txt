cmake_minimum_required(VERSION 3.14)
project(RawVectorTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(raw_vector_tests tests.cpp)

target_link_libraries(raw_vector_tests PRIVATE gtest_main)

include(GoogleTest)
add_test(NAME raw_vector_tests COMMAND raw_vector_tests)