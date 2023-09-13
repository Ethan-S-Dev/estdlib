#include <stdio.h>
#include "list.h"

int main()
{
    int* numbers_list = list_create_cap(int, 6);

    list_add(numbers_list, 5);
    list_add(numbers_list, 4);
    list_add(numbers_list, 2);
    list_add(numbers_list, 6);
    list_add(numbers_list, 9);
    list_add(numbers_list, 4);

    int element = numbers_list[4];

    printf("element at index 4: %d\n", element);

    for (size_t i = 0; i < list_len(numbers_list); i++)
    {
        int number = numbers_list[i];
        printf("element at %zu: %d", i, number);
    }

    list_destroy(numbers_list);

    return 0;
}
