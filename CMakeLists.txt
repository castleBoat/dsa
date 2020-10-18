cmake_minimum_required(VERSION 3.17)
project(cds)

set(CMAKE_CXX_STANDARD 17)

#include_directories(containers/deque)

add_executable(cds main.cpp)

#if (WIN32)
#    set_property(TARGET ${PROJECT_NAME} PROPERTY
#            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
#    target_include_directories(cds PRIVATE ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/include)
#    target_link_libraries(cds ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/build/lib/Debug/gtestd.lib)
#elseif(UNIX)
#    find_package(gtest REQUIRED)
#    target_include_directories(cds PRIVATE ${gtest_INCLUDE_DIRS})
#    target_link_libraries(cds ${gtest_LIBRARIES})
#endif()

enable_testing()
add_subdirectory(test)