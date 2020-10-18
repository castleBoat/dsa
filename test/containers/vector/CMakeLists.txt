file(GLOB SRCS *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/containers/vector)

ADD_EXECUTABLE(testvector ${SRCS})

if (WIN32)
    #    set_property(TARGET ${PROJECT_NAME} PROPERTY
    #            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    set_target_properties(testvector PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    target_include_directories(testvector PRIVATE ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/include)
    target_link_libraries(testvector ${CMAKE_SOURCE_DIR}/../../libs/googletest-release-1.10.0/googletest/build/lib/Debug/*.lib)
elseif(UNIX)
    find_package(gtest REQUIRED)
    target_include_directories(testvector PRIVATE ${gtest_INCLUDE_DIRS})
    target_link_libraries(testvector ${gtest_LIBRARIES})
endif()

add_test(NAME testvector
        COMMAND testvector)