#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

// To jest struktura przechowująca informacje o operacjach na pamięci.
// Nie pozwalamy kompilatorowi optymalizować operacji na tych wartościach.
typedef struct {
    volatile unsigned call_total;    // licznik wszystkich wywołań funkcji
    volatile unsigned call_counter;  // licznik wywołań alokacji
    volatile unsigned fail_counter;  // numer błędnej alokacji
    volatile unsigned alloc_counter; // liczba wykonanych alokacji
    volatile unsigned free_counter;  // liczba wykonanych zwolnień
    volatile char *function_name;    // nazwa nieudanej funkcji
} memory_test_data_t;

// Zapewnia dostęp do wyżej zdefiniowanej struktury.
memory_test_data_t * get_memory_test_data(void);

// Daje wskaźnik na unikalny napis.
char const * get_magic_string(void);

#endif
