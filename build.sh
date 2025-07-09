cmake -S . -B build
cmake --build build

cd build/Debug

start "Calculator_Test" cmd /k Calculator_test.exe

sleep 1m