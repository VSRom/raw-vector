cmake_minimum_required(VERSION 3.14)
project(RawVectorTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Скачивание репозитория напрямую через встроенный Git (БЕЗ использования URL и ZIP)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com"
  GIT_TAG        "v1.14.0"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Собираем исполняемый файл тестов
add_executable(run_tests tests.cpp raw_vector.cpp)

# Подключаем встроенный main из gtest
target_link_libraries(run_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
