file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testdeque ${SRCS})

target_link_libraries(testdeque gtest_main)

add_test(NAME testdeque
        COMMAND testdeque)
