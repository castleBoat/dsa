file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testvector ${SRCS})

target_link_libraries(testvector gtest_main)

add_test(NAME testvector
        COMMAND testvector)