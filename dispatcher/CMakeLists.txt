cmake_minimum_required(VERSION 3.10)

project(dispatcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    dispatcher.cpp
    dispatcheractor.cpp
    dispatcheractors.cpp
    dispatcherthread.cpp
    dispatcherthreads.cpp
    dispatchertimer.cpp
    dispatchertimers.cpp)

set(HDRS
    i_actor.h
    actor.h
    i_dispatcher.h
    dispatcher.h
    dispatcheractor.h
    dispatcheractors.h
    dispatcherthread.h
    dispatcherthreads.h
    dispatchertimer.h
    dispatchertimers.h
    description.h
    dispatcher_common.h)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HDRS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
include_directories (${PROJECT_NAME})

if(BUILD_TESTING)
    set(TEST_DIR test)
    set(TESTS_DIR tests)
    set(HEADERS_TEST
        ${TEST_DIR}/test_common.h
        ${TEST_DIR}/ctestsuite.h
        ${TEST_DIR}/ctestcase.h
        ${TEST_DIR}/${TESTS_DIR}/ctesttimer.h)
    set(SOURCE_TEST
        ${TEST_DIR}/main.cpp
        ${TEST_DIR}/ctestsuite.cpp
        ${TEST_DIR}/ctestcase.cpp
        ${TEST_DIR}/${TESTS_DIR}/ctesttimer.cpp)

    add_executable(dispatcher_test ${SOURCE_TEST} ${HEADERS_TEST})
    target_link_libraries(dispatcher_test dispatcher)
    target_include_directories(dispatcher_test PRIVATE test)

    message("ADDING TESTS")
    # Установить поведение тестов:
    set(TEST_TIMEOUT 1000)

    add_test(NAME TestTimer                 COMMAND dispatcher_test timer)

    set_tests_properties(
        TestTimer
        PROPERTIES TIMEOUT ${TEST_TIMEOUT})

endif()
