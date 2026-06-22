cmake_minimum_required(VERSION 3.14)
project(RawVectorTests LANGUAGES CXX)

# Включаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Скачивание Google Test из официального репозитория через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com
)
# Отключаем лишние внутренние тесты самого gtest для скорости
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Создаем исполняемый файл для тестов
# Сюда входят ваши файлы логики вектора и файл с самими тестами
add_executable(run_tests tests.cpp vsr_vector.cpp)

# Связываем тесты с gtest_main (он сам сгенерирует функцию main для тестов)
target_link_libraries(run_tests PRIVATE gtest_main)

# Интегрируем тесты в экосистему CMake CTest
include(GoogleTest)
gtest_discover_tests(run_tests)
