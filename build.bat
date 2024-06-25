@echo off
rem Let's create the build directory if it doesnt exists
if not exist "build" mkdir build
gcc ./src/uac.c -std=c17 -o ./build/uac.exe -I "./src"


@echo on