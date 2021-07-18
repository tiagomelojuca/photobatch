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

@REM photobatch ^
@REM            --rename ^
@REM            --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
@REM            --filter=.png ^
@REM            --prefix=test_ ^
@REM            --snum=1

photobatch ^
           --convert ^
           --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
           --filter=.png ^
           --from=png ^
           --to=jpg

echo.
