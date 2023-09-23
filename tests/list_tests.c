#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/collections/estd_collections.h"

#define ANSI_FONT_COL_RESET     "\x1b[0m"
#define FONT_COL_RED     "\e[38;2;200;0;0m" // where rrr;ggg;bbb in 38;2;rrr;ggg;bbbm can go from 0 to 255 respectively
#define FONT_COL_GREEN   "\e[38;2;0;200;0m" // where rrr;ggg;bbb in 38;2;rrr;ggg;bbbm can go from 0 to 255 respectively

static size_t _number_of_tests = 0;
static size_t _number_of_tests_passed = 0;
static size_t _number_of_tests_failed = 0;

typedef bool (*test_func)();
void run_test(test_func test, char* name){
    _number_of_tests++;
    printf("running test: %s\n", name);
    if(test()){
        _number_of_tests_passed++;
        return;
    }

    _number_of_tests_failed++;
    printf(FONT_COL_RED "test: %s failed!" ANSI_FONT_COL_RESET "\n", name); 
}

bool test_list_create(){
    estd_list* list = estd_list_create(sizeof(int));
    bool result = list != NULL;
    estd_list_destroy(list);
    return result;
}

bool test_list_add(){
    estd_list* list = estd_list_create(sizeof(int));
    if(list == NULL)
        return false;

    list_add(int, list, 5);
    list_add(int ,list, 4);
    list_add(int, list, 2);
    list_add(int, list, 6);
    list_add(int, list, 9);
    list_add(int, list, 4);

    bool result = list_len(list) == 6;
    result = result && list_items(int, list)[0] == 5;
    result = result && list_items(int, list)[1] == 4;
    result = result && list_items(int, list)[2] == 2;
    result = result && list_items(int, list)[3] == 6;
    result = result && list_items(int, list)[4] == 9;
    result = result && list_items(int, list)[5] == 4;

    estd_list_destroy(list);
    return result;
}

void* mod_5(void* number){
    int num = *(int*)number;
    int* mod5_num_ptr = malloc(sizeof(int));
    *mod5_num_ptr = num % 5;
    return mod5_num_ptr;
}

int main()
{
    printf("Running list tests...\n");
    run_test(test_list_create, "test_list_create");
    run_test(test_list_add, "test_list_add");

    if(_number_of_tests_passed > 0)
        printf(FONT_COL_GREEN "Passed: %zu/%zu tests." ANSI_FONT_COL_RESET "\n", _number_of_tests_passed, _number_of_tests);

    if(_number_of_tests_failed > 0)
        printf(FONT_COL_RED "Failed: %zu/%zu tests." ANSI_FONT_COL_RESET "\n", _number_of_tests_failed, _number_of_tests);

    estd_list* list = estd_list_create(sizeof(int));
    list_add(int, list, 5);
    list_add(int ,list, 4);
    list_add(int, list, 2);
    list_add(int, list, 6);
    list_add(int, list, 9);
    list_add(int, list, 4);

    foreach(int, number, estd_list_iterable(list)){
        printf("number[%zu]: %d\n", iteration.index, iteration.number);
    }

    iterable numbers = estd_list_iterable(list);

    foreach(int, number, numbers){
        printf("number[%zu]: %d\n", iteration.index, iteration.number);
    }
    
    estd_list_destroy(list);
    printf("List tests finished!\n");
}
