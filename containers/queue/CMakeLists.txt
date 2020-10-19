file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testqueue ${SRCS})

target_link_libraries(testqueue gtest_main)

add_test(NAME testqueue
        COMMAND testqueue)