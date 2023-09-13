@echo off

:: Compile list.c into an object file
clang -c list.c -o list.o

:: Compile list_test.c into an object file
clang -c list_tests.c -o list_tests.o

:: Link object files to create an executable
clang list.o list_tests.o -o list_tests.exe 

:: Remove intermediate object files
del list.o
del list_tests.o

echo.
echo Build completed.
