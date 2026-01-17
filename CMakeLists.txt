cmake_minimum_required(VERSION 3.10)
project(projekt)

# Require C++26 (or C++23 if 26 is too new for your compiler)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MyProgram)

target_sources(MyProgram
        PRIVATE
        main.cpp
        Uzytkownik.h
        Uzytkownik.cpp
        Ksiazka.cpp
        Ksiazka.h
        Display.h
        Display.cpp
)

if(MINGW)
    target_link_libraries(MyProgram PRIVATE stdc++exp)
endif()
