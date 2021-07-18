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

photobatch --help

@REM photobatch ^
@REM            --rename ^
@REM            --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
@REM            --filter=.png ^
@REM            --prefix=test_ ^
@REM            --snum=1

@REM photobatch ^
@REM            --convert ^
@REM            --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
@REM            --filter=.jpg ^
@REM            --from=jpg ^
@REM            --to=png

@REM photobatch ^
@REM            --resize ^
@REM            --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
@REM            --filter=.jpg ^
@REM            --width=500 ^
@REM            --height=500

@REM photobatch ^
@REM            --scale ^
@REM            --folder=C:\Users\Tiago\Desktop\TMP\ccp-posts ^
@REM            --filter=.jpg ^
@REM            --amount=2

echo.
