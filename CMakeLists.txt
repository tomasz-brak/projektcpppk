cmake_minimum_required(VERSION 3.10)
project(projekt)

set(CMAKE_CXX_STANDARD 26)

add_library(projekt_lib
        src/Serializer/Serializable.cpp
        src/Serializer/Serializable.h
        src/Serializer/storeClassList.cpp
        src/Serializer/storeClassList.h
        src/Serializer/ISerializable.cpp
        src/Serializer/ISerializable.h
        src/util.cpp
        src/util.h
        src/Serializer/Token.cpp
        src/Serializer/Token.h
)

add_executable(projekt
        src/main.cpp
        src/Klient.cpp
        src/Klient.h
)

target_link_libraries(projekt projekt_lib)

enable_testing()

add_executable(tests
        tests/bootstrap.cpp
        tests/test_util.h
        tests/test_Serializer.cpp
        tests/test_Serializer.h
)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)

target_link_libraries(tests projekt_lib)

add_test(
        NAME testAll
        COMMAND tests
)