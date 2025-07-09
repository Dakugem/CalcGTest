cmake -S . -B build -D ENABLE_COVERAGE=ON
cmake --build build

ctest -V

cmake --build build --target coverage
rem "build\Debug\Calculator_test.exe"