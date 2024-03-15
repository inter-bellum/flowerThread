set(CMAKE_MINIMUM_REQUIRED_VERSION 3.19)

project(test_flowerthread_funcs)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_flowerthreads
    main.cpp
    Module.cpp
    mock.cpp
)

target_include_directories(test_flowerthreads PRIVATE .)
target_compile_definitions(test_flowerthreads PRIVATE TEST_LOCAL)
