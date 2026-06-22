cmake_minimum_required(VERSION 3.14)
project(RawVectorTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем установку gtest в систему и сборку лишних тестов
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "" OFF)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

# Просто подключаем локальную папку подмодуля googletest
add_subdirectory(googletest EXCLUDE_FROM_ALL)

enable_testing()

# Собираем исполняемый файл тестов
add_executable(run_tests tests.cpp raw_vector.cpp)

# Привязываем Google Test к нашему исполняемому файлу
target_link_libraries(run_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
