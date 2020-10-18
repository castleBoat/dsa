file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testmain ${SRCS} containers/queue/queue_unittest.cpp)

#TARGET_LINK_LIBRARIES(testmain
#        libfoo
#        libgtest
#        libgmock
#        )

if (WIN32)
#    set_property(TARGET ${PROJECT_NAME} PROPERTY
#            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    set_target_properties(testmain PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    target_include_directories(testmain PRIVATE ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/include)
    target_link_libraries(testmain ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/build/lib/Debug/*.lib)
elseif(UNIX)
    find_package(gtest REQUIRED)
    target_include_directories(testmain PRIVATE ${gtest_INCLUDE_DIRS})
    target_link_libraries(testmain ${gtest_LIBRARIES})
endif()

add_test(NAME testmain
        COMMAND testmain)

add_subdirectory(containers)