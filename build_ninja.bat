cmake -S . -B build -G Ninja -D ENABLE_COVERAGE=ON -D CMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" -D CMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe"
cmake --build build



rem ctest -V -T Coverage -C Release --test-dir build

rem cmake --build build --target coverage