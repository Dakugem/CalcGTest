cmake_minimum_required(VERSION 3.22)
project(CalcGTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CalcLib
        ICalculator.h
        SimpleCalculator.h SimpleCalculator.cpp
        IHistory.h
        InMemoryHistory.h InMemoryHistory.cpp
)


include(FetchContent)
FetchContent_Declare(
	googletest
	#GIT_REPOSITORY https://github.com/google/googletest.git
	#URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
	URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)

enable_testing()

add_executable(
	Calculator_test
	CalculatorTest.cpp
)

target_link_libraries (
	Calculator_test
	CalcLib
	GTest::gtest_main
)