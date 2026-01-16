cmake_minimum_required(VERSION 3.10)
project(projekt)

set(CMAKE_CXX_STANDARD 26)


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


