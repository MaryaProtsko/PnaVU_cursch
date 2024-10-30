cmake_minimum_required(VERSION 3.26)
project(lab_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_2 source/main.cpp
        source/AnxietyTest.cpp
        header/AnxietyTest.h
        source/AnxietyCopingSuggestions.cpp
        header/AnxietyCopingSuggestions.h
        source/SituationalAnxietyTest.cpp
        header/SituationalAnxietyTest.h
        source/PersonalAnxietyTest.cpp
        header/PersonalAnxietyTest.h
        source/UserManager.cpp
        header/UserManager.h
        source/globalFuncs.cpp
        header/globalFuncs.h
        header/AnxietyTest.h
        header/AnxietyCopingSuggestions.h
        header/globalFuncs.h
        header/PersonalAnxietyTest.h
        header/SituationalAnxietyTest.h
        header/UserManager.h)
