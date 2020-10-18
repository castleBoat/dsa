file(GLOB SRCS *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/containers/queue)

ADD_EXECUTABLE(testqueue ${SRCS})

if (WIN32)
    #    set_property(TARGET ${PROJECT_NAME} PROPERTY
    #            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    set_target_properties(testqueue PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    target_include_directories(testqueue PRIVATE ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/include)
    target_link_libraries(testqueue ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/build/lib/Debug/*.lib)
elseif(UNIX)
    find_package(gtest REQUIRED)
    target_include_directories(testqueue PRIVATE ${gtest_INCLUDE_DIRS})
    target_link_libraries(testqueue ${gtest_LIBRARIES})
endif()

add_test(NAME testqueue
        COMMAND testqueue)