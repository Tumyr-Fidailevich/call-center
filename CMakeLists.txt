cmake_minimum_required(VERSION 3.25)
#set(CMAKE_TOOLCHAIN_FILE "C:/Users/timur/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(CallCenter)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES src/http_server.cpp)
set(HEADERS include/http_server.h include/cdr.h)

add_library(CallCenterLib STATIC ${SOURCES} ${HEADERS})
target_include_directories(CallCenterLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_precompile_headers(CallCenterLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h)

add_executable(CallCenterExec src/main.cpp)


find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED)
find_package(glog CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(CallCenterExec PRIVATE CallCenterLib ${Boost_LIBRARIES} nlohmann_json::nlohmann_json glog::glog)

#option(BUILD_TESTS "Build tests" OFF)
#
#if (BUILD_TESTS)
set(TEST_SOURCES test/test.cpp)

add_executable(call_center_test ${TEST_SOURCES})
find_package(GTest REQUIRED)
target_link_libraries(call_center_test PRIVATE CallCenterLib GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
add_test(NAME CallCenterTest COMMAND call_center_test)

#endif ()