set(CMAKE_MINIMUM_REQUIRED_VERSION 3.19)

project(test_flowerthread_funcs)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_flowerthreads
    mock/main.cpp
    Module.cpp
    mock/mock.cpp
)

target_include_directories(test_flowerthreads PRIVATE . mock)
target_compile_definitions(test_flowerthreads PRIVATE TEST_LOCAL)
