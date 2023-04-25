#include "memory_tests.h"
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
char const * get_magic_string(void) {
    static char const * magic = "quite long magic string";
    return magic;
}

/*
// Ten plik musi kompilowany z opcjami -std=gnu17 i -fPIC,
// a linkowany z opcjami -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc
// -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup.

// Przechwytujemy funkcje alokujące i zwalniające pamięć.
void * __real_malloc(size_t size) __attribute__((weak));
void * __real_calloc(size_t nmemb, size_t size) __attribute__((weak));
void * __real_realloc(void *ptr, size_t size) __attribute__((weak));
void * __real_reallocarray(void *ptr, size_t nmemb, size_t size) __attribute__((weak));
char * __real_strdup(const char *s) __attribute__((weak));
char * __real_strndup(const char *s, size_t size) __attribute__((weak));
void   __real_free(void *ptr) __attribute__((weak));

// Trzymamy globalnie informacje o alokacjach i zwolnieniach pamięci.
static memory_test_data_t test_data;

// To jest prosty akcesor potrzebny do testowania.
memory_test_data_t * get_memory_test_data(void) {
    return &test_data;
}

// To jest prosty akcesor potrzebny do testowania.
char const * get_magic_string(void) {
    static char const * magic = "quite long magic string";
    return magic;
}

// W zadanym momencie alokacja pamięci zawodzi.
static bool should_fail(void) {
    return ++test_data.call_counter == test_data.fail_counter;
}

// Realokacja musi się udać, jeśli nie zwiększamy rozmiaru alokowanej pamięci.
static bool can_fail(void const *old_ptr, size_t new_size) {
    if (old_ptr == NULL)
        return true;
    else
        return new_size > malloc_usable_size((void *)old_ptr);
}

// Symulujemy brak pamięci.
#define UNRELIABLE_ALLOC(ptr, size, fun, name)                           \
  do {                                                                   \
    test_data.call_total++;                                              \
    if (ptr != NULL && size == 0) {                                      \
      /* Takie wywołanie realloc jest równoważne wywołaniu free(ptr). / \
      test_data.free_counter++;                                          \
      return fun;                                                        \
    }                                                                    \
    void *p = can_fail(ptr, size) && should_fail() ? NULL : (fun);       \
    if (p) {                                                             \
      test_data.alloc_counter += ptr != p;                               \
      test_data.free_counter += ptr != p && ptr != NULL;                 \
    }                                                                    \
    else {                                                               \
      errno = ENOMEM;                                                    \
      test_data.function_name = name;                                    \
    }                                                                    \
    return p;                                                            \
  } while (0)

void *__wrap_malloc(size_t size) {
    UNRELIABLE_ALLOC(NULL, size, __real_malloc(size), "malloc");
}

void *__wrap_calloc(size_t nmemb, size_t size) {
    UNRELIABLE_ALLOC(NULL, nmemb * size, __real_calloc(nmemb, size), "calloc");
}

void *__wrap_realloc(void *ptr, size_t size) {
    UNRELIABLE_ALLOC(ptr, size, __real_realloc(ptr, size), "realloc");
}

void *__wrap_reallocarray(void *ptr, size_t nmemb, size_t size) {
    UNRELIABLE_ALLOC(ptr, nmemb * size, __real_reallocarray(ptr, nmemb, size), "reallocarray");
}

char *__wrap_strdup(const char *s) {
    UNRELIABLE_ALLOC(NULL, 0, __real_strdup(s), "strdup");
}

char *__wrap_strndup(const char *s, size_t size) {
    UNRELIABLE_ALLOC(NULL, 0, __real_strndup(s, size), "strndup");
}

// Zwalnianie pamięci zawsze się udaje. Odnotowujemy jedynie fakt zwolnienia.
void __wrap_free(void *ptr) {
    test_data.call_total++;
    __real_free(ptr);
    if (ptr)
        test_data.free_counter++;
}
*/