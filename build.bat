@echo off
:: Create the bin folder if it doesn't exist
if not exist bin mkdir bin

:: Compile the C source files into object files
clang -c ./src/disposable/disposable.c -o ./bin/disposable.o
clang -c ./src/collections/enumerable/enumerator.c -o ./bin/enumerator.o
clang -c ./src/collections/enumerable/enumerable.c -o ./bin/enumerable.o
clang -c ./src/collections/enumerable/filter/filter.c -o ./bin/enumerable_filter.o
clang -c ./src/collections/enumerable/map/map.c -o ./bin/enumerable_map.o

clang -c ./src/collections/list/list.c -o ./bin/list.o
clang -c ./src/collections/linked_list/linked_list.c -o ./bin/linked_list.o

:: Create the static library
llvm-ar rcs ./bin/collections.lib ./bin/disposable.o ./bin/enumerator.o ./bin/enumerable.o ./bin/enumerable_filter.o ./bin/enumerable_map.o ./bin/list.o ./bin/linked_list.o

:: Delete the temporary object files
del bin\*.o

@echo on

