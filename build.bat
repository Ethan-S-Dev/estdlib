@echo off
:: Create the bin folder if it doesn't exist
if not exist bin mkdir bin

:: Compile the C source files into object files
clang -c ./src/collections/list/list.c -o ./bin/list.o
clang -c ./src/collections/iterator/iterator.c -o ./bin/iterator.o
clang -c ./src/collections/iterator/filter/filter_iterator.c -o ./bin/filter_iterator.o
clang -c ./src/collections/iterator/map/map_iterator.c -o ./bin/map_iterator.o

:: Create the static library
llvm-ar rcs ./bin/collections.lib ./bin/iterator.o ./bin/filter_iterator.o ./bin/map_iterator.o ./bin/list.o

:: Delete the temporary object files
del bin\*.o

@echo on

