@echo off

:: Execute the previous batch file to build the library
call build.bat

:: Compile the main C file into an object file in the bin folder
clang -c ./tests/list_tests.c -o bin\list_tests.o

:: Link the object file with the static library to create the executable
clang -v bin\list_tests.o -L./bin -l./bin/collections.lib -o bin\list_tests.exe

:: Optionally, delete the temporary object files from the bin folder
:: del bin\list_tests.o

@echo on
