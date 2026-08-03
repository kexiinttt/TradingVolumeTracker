cmake_minimum_required(VERSION 3.15)

project(SimpleTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(simple_tracker_tests
    main.cpp
    src/simpleTracker.cpp
    src/anyQueryTracker.cpp
)

target_include_directories(simple_tracker_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/header
)

enable_testing()
add_test(NAME SimpleTrackerTest COMMAND simple_tracker_tests)
