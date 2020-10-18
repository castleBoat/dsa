file(GLOB SRCS *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/containers/deque)

ADD_EXECUTABLE(testdeque ${SRCS})

#TARGET_LINK_LIBRARIES(testdeque
#        libfoo
#        libgtest
#        libgmock
#        )

if (WIN32)
    #    set_property(TARGET ${PROJECT_NAME} PROPERTY
    #            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    set_target_properties(testdeque PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    target_include_directories(testdeque PRIVATE ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/include)
    target_link_libraries(testdeque ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/build/lib/Debug/*.lib)
elseif(UNIX)
    find_package(gtest REQUIRED)
    target_include_directories(testdeque PRIVATE ${gtest_INCLUDE_DIRS})
    target_link_libraries(testdeque ${gtest_LIBRARIES})
endif()

add_test(NAME testdeque
        COMMAND testdeque)
