@echo off
if not exist "build" mkdir "build"
cd "build"
cmake -G "MinGW Makefiles" ..
mingw32-make

echo.
echo +----------------------------------------+
echo ^| RUNNING OUTPUT EXECUTABLE              ^|
echo +----------------------------------------+
echo.

photobatch ^
           --rename ^
           --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
           --filter=.png ^
           --prefix=test_ ^
           --snum=1

echo.