#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

#define N 5

int random_int(int from, int to) {
    return from + (rand() % (to - from));
}

int main()
{
    srand(time(NULL));

    int** numbers_list = list_create(int*);

    printf("inserting numbers to the list...\n");
    for (size_t i = 0; i < N; i++){
        int* int_ptr = malloc(sizeof(*int_ptr));
        if (int_ptr == NULL)
            return -1;
        *int_ptr = random_int(100, 2000);
        list_add(numbers_list, int_ptr);
    }
    printf("Numbers in list:\n");
    for (size_t i = 0; i < list_len(numbers_list); i++) {
        int* number_ptr = numbers_list[i];
        printf("[%zu]: %p -> %d\n", i, number_ptr, *number_ptr);
    }
    printf("Removing numbers from list...\n");
    for (size_t i = 1; i < list_len(numbers_list); i = i * 2) {
        int* number_ptr = numbers_list[i];
        printf("Removing pointer from [%zu]\n", i);
        list_remove(numbers_list, i);
        free(number_ptr);
    }
    printf("Numbers in list:\n");
    for (size_t i = 0; i < list_len(numbers_list); i++) {
        int* number_ptr = numbers_list[i];
        printf("[%zu]: %p -> %d\n", i, number_ptr, *number_ptr);
    }
    printf("Cleaning numbers from list...\n");
    for (size_t i = 0; i < list_len(numbers_list);) {
        int* number_ptr = numbers_list[i];
        printf("Removing pointer from [%zu]\n", i);
        list_remove(numbers_list, i);
        free(number_ptr);
    }

    list_destroy(numbers_list);

    return 0;
}
