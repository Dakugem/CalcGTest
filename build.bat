cmake -S . -B build -G Ninja -D ENABLE_COVERAGE=ON
cmake --build build

cd build

ctest -T Test -T Coverage

cd ..