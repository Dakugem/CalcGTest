cmake -S . -B build -G Ninja -D ENABLE_COVERAGE=ON
cmake --build build

cd build

ctest -V -T Test 
ctest -T Coverage

cd ..