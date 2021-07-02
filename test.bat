@echo off
if not exist "build" mkdir "build"
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .

cd tests
TestArgumentParser.exe --gtest_color=yes
