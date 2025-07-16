cmake_minimum_required(VERSION 3.14)
project(StockProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stock_lib src/stock.cpp)
add_library(cer_lib src/cer.cpp)
add_library(person_lib src/person.cpp)

target_include_directories(stock_lib PUBLIC include)
target_include_directories(cer_lib PUBLIC include)
target_include_directories(person_lib PUBLIC include)

add_executable(main_app ./main.cpp)
target_link_libraries(main_app PRIVATE person_lib stock_lib cer_lib)

# google test setting
enable_testing()
include(CTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
add_executable(runTests tests/stock_gtest.cpp)
target_link_libraries(runTests PRIVATE stock_lib cer_lib person_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(runTests)