@echo off
:: Create the bin folder if it doesn't exist
if not exist bin mkdir bin

:: Compile the C source files into object files
clang -c ./src/disposable/disposable.c -o ./bin/disposable.o
clang -c ./src/collections/iterable/iterator.c -o ./bin/iterator.o
clang -c ./src/collections/iterable/iterable.c -o ./bin/iterable.o

clang -c ./src/collections/list/list.c -o ./bin/list.o
clang -c ./src/collections/linked_list/linked_list.c -o ./bin/linked_list.o

:: Create the static library
llvm-ar rcs ./bin/collections.lib ./bin/disposable.o ./bin/iterator.o ./bin/iterable.o ./bin/list.o ./bin/linked_list.o

:: Delete the temporary object files
del bin\*.o

@echo on

