@echo off
rem Let's create the build directory if it doesnt exists
if not exist "build" mkdir build
gcc ./src/uac.c -std=c17 -o ./build/uac.exe -I "./src"

rem The start of the build asset system, not done
cd build

rem tf you gonna put there, loading screens? it's a fucking compiler
if not exist "assets" mkdir assets

cd ..


@echo on