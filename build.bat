@echo off
rem Let's create the build directory if it doesnt exists
if not exist "build" mkdir build
gcc ./src/uac.c -std=c17 -o ./build/uac.exe -I "./src"

cd src
set SOURCE_PATH=%CD%
cd scripts
set SOURCE_SCRIPTS_PATH=%CD%
cd ..
cd ..

rem The start of the build asset system, not done
cd build

rem tf you gonna put there, loading screens? it's a fucking compiler
if not exist "assets" mkdir assets
if not exist "scripts" mkdir scripts
cd scripts
copy /Y "%SOURCE_SCRIPTS_PATH%\tokenizer_debug.py" "%CD%\tokenizer_debug.py" > NUL
cd ..
cd ..


@echo on