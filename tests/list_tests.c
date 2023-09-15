#include <stdio.h>
#include <stdbool.h>
#include "../src/collections/estd_collections.h"

typedef struct person_t{
    int age;
} person_t;

static void print_number(void* number_p){
    int number = *(int*)number_p;
    printf("this is number: %d\n", number);
}

static void print_person(void* person_p) {
    person_t* person = (person_t*)person_p;
    printf("this is people age: %d\n", person->age);
}

static bool grater_then(void* then_p, void* item_p){
    int then = *(int*)then_p;
    int item = *(int*)item_p;
    return item > then;
}

static void* to_person(void* number_p){
    int number = *(int*)number_p;
    person_t* p = malloc(sizeof(person_t));
    if(p == NULL) return NULL;
    p->age = number;
    return p;
}

int main()
{
    list_t* numbers_list = estd_list_create(sizeof(int));

    list_add(int, numbers_list, 5);
    list_add(int ,numbers_list, 4);
    list_add(int, numbers_list, 2);
    list_add(int, numbers_list, 6);
    list_add(int, numbers_list, 9);
    list_add(int, numbers_list, 4);

    int element = list_items(int, numbers_list)[4];

    printf("element at index 4: %d\n", element);

    iterator_t numbers = estd_list_iterator(numbers_list);

    foreach(&numbers, print_number);

    int four = 4;
    iterator_t filtered = estd_iter_filter_args(&numbers, grater_then, &four);
    iterator_t people = estd_iter_map(&filtered, to_person, sizeof(person_t));
    
    foreach(&people, print_person);

    list_t* people_list = estd_list_create_iter(&people);

    printf("number of people: %zu\n", list_len(people_list));

    estd_iter_dispose(&numbers);
    estd_iter_dispose(&filtered);
    estd_iter_dispose(&people);
    estd_list_destroy(numbers_list);
    estd_list_destroy(people_list);
    return 0;
}