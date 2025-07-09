cmake_minimum_required(VERSION 3.22)
project(CalcGTest LANGUAGES CXX)

include(CTest)

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
	URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
FetchContent_MakeAvailable (googletest)

enable_testing()

add_executable(
	Calculator_test
	CalculatorTest.cpp
)

target_link_libraries (
	Calculator_test
		PRIVATE
	CalcLib
	gtest_main
)

add_test(NAME UnitTests COMMAND CalculatorTest)