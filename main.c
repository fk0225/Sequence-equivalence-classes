/*#include <stdio.h>
#include <string.h>
#include "seq.h"
#include "dsf.h"

int main() {
    seq_t *p = seq_new();
    printf("seq_add=%d\n",seq_add(p, "012"));
    seq_add(p, "0122");
    //printf("%d\n",seq_remove(p, "0122"));
    seq_set_name(p,"012",strdup("seq0"));
    seq_set_name(p,"0122",strdup("seq1"));
    printf("%d\n",seq_valid(p, "012"));
    //p->classNames->currCapacity;
    //printf("Size=%d\n",);
    printf("%d!!!!!!!!!!!!!!\n",seq_equiv(p,"012","0122"));
    puts(seq_get_name(p,"0122"));
    puts("!!!!!!!!!!!!!!!");
    seq_add(p,"120");
    printf("%d\n",seq_set_name(p,"120",strdup("seq2")));
    printf("%d???????????????????????????\n",seq_equiv(p,"012","120"));
    printf("%d\n",seq_remove(p, "012"));
    printf("%d\n",seq_remove(p, "0122"));
    puts(seq_get_name(p,"120"));
    puts("RIGHT ABOVE");
    seq_delete(p);
    UFNode * xd=newUFNode(strdup("xd")), * yd=newUFNode(strdup("yd")), *zd= newUFNode(NULL), *wd= newUFNode(NULL);
    printClassName(findWithPathComp(xd));
    printClassName(findWithPathComp(yd));
    printClassName(findWithPathComp(zd));
    unionByRank(findWithPathComp(wd),findWithPathComp(zd));
    printNodeRank(findWithPathComp(wd));
    printClassName(findWithPathComp(wd));
    unionByRank(xd,yd);
    printNodeRank(findWithPathComp(yd));
    printf("%d\n",unionByRank(xd,zd));
   // printNodeRank(findWithPathComp(zd)); ///DECREASE RANK AT FINDS?????!!!!!
    printClassName(findWithPathComp(xd));
    printClassName(findWithPathComp(yd));
    safeDeleteUFNode(xd);
    safeDeleteUFNode(yd);

    safeDeleteUFNode(wd);
    safeDeleteUFNode(zd);
    return 0;
}///SOURCE OF SIGTRAP: UNIONBYRANK*/

#include <assert.h>
#include <stdlib.h>
#include "seq.h"
#include "memory_tests.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>



// To są możliwe wyniki testu.
#define PASS 0
#define FAIL (-1)
#define WRONG_TEST 2

// Oblicza liczbę elementów tablicy x.
#define SIZE(x) (sizeof x / sizeof x[0])

#define CHECK(x)\
if (!(x)) { \
  printf("TEST %d, CHECK %d NIE PRZESZEDL\n", testid, counter);\
  _Exit(0);\
}\
else { \
  ++counter;\
}


#define TEST_EINVAL(f)                  \
  do {                                  \
    if ((f) != -1 || errno != EINVAL)   {  \
     /* printf("currTestNo=%d\n",currTestNo);      */                            \
      return FAIL;                      \
    } \
  } while (0)

#define TEST_NULL_EINVAL(f)             \
  do {                                  \
    if ((f) != NULL || errno != EINVAL) {  \
     /* printf("currTestNo=%d\n",currTestNo);   */                               \
      return FAIL;                      \
    } \
  } while (0)

#define TEST_PASS(f)                    \
  do {                                  \
    if ((f) != 1)                       {  \
     /* printf("currTestNo=%d\n",currTestNo);  */                                \
      return FAIL;                      \
    } \
  } while (0)

#define TEST_FAIL(f)                    \
  do {                                  \
    if ((f) != 0)                       {  \
      /*printf("currTestNo=%d\n",currTestNo);*/                                  \
      return FAIL;                      \
    } \
} while (0)

#define TEST_COMP(f, s)                 \
  do {                                  \
    if (strcmp((f), (s)) != 0)          {  \
     /* printf("currTestNo=%d\n",currTestNo);  */                                \
      return FAIL;                      \
    } \
} while (0)

#define TEST_NULL_FAIL(f)               \
  do {                                  \
    if ((f) != NULL || errno != 0)   {  \
      /*printf("currTestNo=%d\n",currTestNo);*/                                  \
      return FAIL;                      \
    }                                   \
  } while (0)

/// THE CASE WHEN ERRNO SET TO 0 !!!

#define V(code, where) (((unsigned long)code) << (3 * where))



// Testuje poprawność weryfikacji parametrów wywołań funkcji.
static int params(void) {
    static const char bad_seq[4] = {0, 1, 2, 3};

    int currTestNo=0;
    seq_t *seq = seq_new();

    TEST_EINVAL(seq_add(NULL, "1"));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, NULL));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, ""));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, "/"));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, "3"));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, "10/"));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, "103"));
    currTestNo++;
    TEST_EINVAL(seq_add(seq, bad_seq));
    currTestNo++;

    TEST_EINVAL(seq_remove(NULL, "2"));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, NULL));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, ""));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, "/"));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, "3"));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, "21/"));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, "213"));
    currTestNo++;
    TEST_EINVAL(seq_remove(seq, bad_seq));
    currTestNo++;

    TEST_EINVAL(seq_valid(NULL, "0"));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, NULL));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, ""));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, "/"));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, "3"));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, "0/0"));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, "030"));
    currTestNo++;
    TEST_EINVAL(seq_valid(seq, bad_seq));
    currTestNo++;

    TEST_EINVAL(seq_set_name(NULL, "0", "a"));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, NULL, "b"));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, "", "c"));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, "1", NULL));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, "2", ""));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, "/", "d"));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, "3", "e"));
    currTestNo++;
    TEST_EINVAL(seq_set_name(seq, bad_seq, "f"));
    currTestNo++;

    TEST_NULL_EINVAL(seq_get_name(NULL, "0"));
    currTestNo++;
    TEST_NULL_EINVAL(seq_get_name(seq, NULL));
    currTestNo++;
    TEST_NULL_EINVAL(seq_get_name(seq, ""));
    currTestNo++;
    TEST_NULL_EINVAL(seq_get_name(seq, "/"));
    currTestNo++;
    TEST_NULL_EINVAL(seq_get_name(seq, "3"));
    currTestNo++;
    TEST_NULL_EINVAL(seq_get_name(seq, bad_seq));
    currTestNo++;

    TEST_EINVAL(seq_equiv(NULL, "0", "1"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, NULL, "1"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "", "1"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "0", NULL));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "0", ""));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "/", "1"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "3", "1"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "0", "/"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "0", "3"));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, "0", bad_seq));
    currTestNo++;
    TEST_EINVAL(seq_equiv(seq, bad_seq, "1"));
    currTestNo++;

    seq_delete(seq);
    return PASS;
}

// Testuje podstawową funkcjonalność biblioteki.
static int simple(void) {
    seq_t *seq = seq_new();

    int currTestNo=0;
    TEST_PASS(seq_add(seq, "012"));
    currTestNo++;
    TEST_FAIL(seq_add(seq, "01"));
    currTestNo++;
    TEST_FAIL(seq_remove(seq, "0120"));
    currTestNo++;

    TEST_PASS(seq_valid(seq, "0"));
    currTestNo++;
    TEST_PASS(seq_valid(seq, "01"));
    currTestNo++;
    TEST_PASS(seq_valid(seq, "012"));
    currTestNo++;
    TEST_FAIL(seq_valid(seq, "0120"));
    currTestNo++;
    TEST_PASS(seq_remove(seq, "01"));
    currTestNo++;
    TEST_PASS(seq_valid(seq, "0"));
    currTestNo++;
    TEST_FAIL(seq_valid(seq, "01"));
    currTestNo++;
    TEST_FAIL(seq_valid(seq, "012"));
    currTestNo++;

    seq_delete(seq);
    return PASS;
}
/*
// Testuje tworzenie klas abstrakcji i przypisywanie im nazw.
static int equivalence(void) {
    seq_t *seq = seq_new();

    int currTestNo=0;
    TEST_FAIL(seq_equiv(seq, "0", "1"));
    currTestNo++;

    TEST_PASS(seq_add(seq, "00"));
    currTestNo++;
    TEST_FAIL(seq_equiv(seq, "00", "00"));///!!!
    currTestNo++;
    TEST_FAIL(seq_equiv(seq, "00", "11"));
    currTestNo++;

    TEST_PASS(seq_set_name(seq, "0", "zero"));
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "0"), "zero");
    currTestNo++;
    TEST_FAIL(seq_set_name(seq, "0", "zero"));///!!!
    currTestNo++;
    TEST_PASS(seq_set_name(seq, "0", "ZERO"));
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "0"), "ZERO");
    currTestNo++;
    TEST_FAIL(seq_set_name(seq, "000", "trzy zera"));
    currTestNo++;
    //puts("After trzy zera");
    TEST_NULL_FAIL(seq_get_name(seq, "00"));///!!!
    currTestNo++;
    TEST_NULL_FAIL(seq_get_name(seq, "1"));
    currTestNo++;

    TEST_PASS(seq_add(seq, "11"));
    currTestNo++;

    TEST_NULL_FAIL(seq_get_name(seq, "1"));
    currTestNo++;
    TEST_NULL_FAIL(seq_get_name(seq, "11"));
    currTestNo++;

    TEST_PASS(seq_equiv(seq, "0", "1"));
    currTestNo++;
  //  puts("After pass equiv 0 1");
    TEST_FAIL(seq_equiv(seq, "0", "1"));///!!!
    currTestNo++;
 //   puts("After fail equiv 0 1");

    TEST_COMP(seq_get_name(seq, "0"), "ZERO");
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "1"), "ZERO");
    currTestNo++;

    TEST_PASS(seq_equiv(seq, "00", "11"));
    currTestNo++;

    TEST_PASS(seq_set_name(seq, "1", "JEDEN"));
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "0"), "JEDEN");
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "1"), "JEDEN");
    currTestNo++;
    TEST_PASS(seq_set_name(seq, "11", "DWA"));
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "00"), "DWA");
    currTestNo++;
   // puts("Before last DWA");
    TEST_COMP(seq_get_name(seq, "11"), "DWA");
    currTestNo++;
  //  puts("After last DWA");

    TEST_PASS(seq_equiv(seq, "11", "0"));
    currTestNo++;
  //  puts("Before first DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "0"), "DWAJEDEN");
    currTestNo++;
 //   puts("After first DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "1"), "DWAJEDEN");
    currTestNo++;
//    puts("After second DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "00"), "DWAJEDEN");
    currTestNo++;
    TEST_COMP(seq_get_name(seq, "11"), "DWAJEDEN");
    currTestNo++;

    TEST_FAIL(seq_equiv(seq, "11", "11"));
    currTestNo++;
  //  puts("AFTER LAST TEST ALL PASSED");
    seq_delete(seq);
    return PASS;
}*/


static int equivalence(void) {
    seq_t * seq = seq_new();

    TEST_FAIL(seq_equiv(seq, "0", "1"));

    TEST_PASS(seq_add(seq, "00"));
    TEST_FAIL(seq_equiv(seq, "00", "00"));
    TEST_FAIL(seq_equiv(seq, "00", "11"));

    TEST_PASS(seq_set_name(seq, "0", "zero"));
    TEST_COMP(seq_get_name(seq, "0"), "zero");
    TEST_FAIL(seq_set_name(seq, "0", "zero"));
    TEST_PASS(seq_set_name(seq, "0", "ZERO"));
    TEST_COMP(seq_get_name(seq, "0"), "ZERO");
    TEST_FAIL(seq_set_name(seq, "000", "trzy zera"));
    TEST_NULL_FAIL(seq_get_name(seq, "00"));
    TEST_NULL_FAIL(seq_get_name(seq, "1"));

    TEST_PASS(seq_add(seq, "11"));

    TEST_NULL_FAIL(seq_get_name(seq, "1"));
    TEST_NULL_FAIL(seq_get_name(seq, "11"));

    TEST_PASS(seq_equiv(seq, "0", "1"));
    TEST_FAIL(seq_equiv(seq, "0", "1"));

    TEST_COMP(seq_get_name(seq, "0"), "ZERO");
    TEST_COMP(seq_get_name(seq, "1"), "ZERO");

    TEST_PASS(seq_equiv(seq, "00", "11"));

    TEST_PASS(seq_set_name(seq, "1", "JEDEN"));
    TEST_COMP(seq_get_name(seq, "0"), "JEDEN");
    TEST_COMP(seq_get_name(seq, "1"), "JEDEN");
    TEST_PASS(seq_set_name(seq, "11", "DWA"));
    TEST_COMP(seq_get_name(seq, "00"), "DWA");
    TEST_COMP(seq_get_name(seq, "11"), "DWA");

    TEST_PASS(seq_equiv(seq, "11", "0"));
    TEST_COMP(seq_get_name(seq, "0"), "DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "1"), "DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "00"), "DWAJEDEN");
    TEST_COMP(seq_get_name(seq, "11"), "DWAJEDEN");

    TEST_FAIL(seq_equiv(seq, "11", "11"));

    seq_delete(seq);
    return PASS;
}


static int custom(void) {

    if (1) {
        seq_t * t = seq_new();
        const int testid = 1;
        int counter = 0;
        const int big = 1300000;
        char * str = malloc(sizeof(char) * (big + 1));
        assert(!!str);

        for (int i = 0; i < big; i++)
            str[i] = '0' + (i % 3);
        str[big] = 0;

        int res = seq_add(t, str);
        CHECK(res == 1);

        res = seq_set_name(t, str, str);
        CHECK(res == 1);

        res = seq_equiv(t, str, "012012012");
        CHECK(res == 1);

        res = seq_valid(t, str);
        CHECK(res == 1);

        const char * res2 = seq_get_name(t, str);
        CHECK(strcmp(res2, str) == 0);

        const char * res3 = seq_get_name(t, "012012012");
        CHECK(strcmp(res3, str) == 0);

        str[big / 2] = 0;

        res = seq_remove(t, str);
        CHECK(res == 1);

        res = seq_remove(t, str);
        CHECK(res == 0);

        seq_delete(t);

        free(str);

        printf("TEST %d OK\n", testid);
    }

    printf("PRZESZLO!\n");
    return PASS;
}

// Testuje reakcję implementacji na niepowodzenie alokacji pamięci.
static unsigned long alloc_fail_seq_new_seq_add(void) {
    unsigned long visited = 0;
    seq_t *seq;
    int result;

    if ((seq = seq_new()) != NULL)
        visited |= V(1, 0);
    else if (errno == ENOMEM && (seq = seq_new()) != NULL)
        visited |= V(2, 0);
    else
        return visited |= V(4, 0);

    if ((result = seq_add(seq, "012")) == 1)
        visited |= V(1, 1);
    else if (result == -1 && errno == ENOMEM && seq_valid(seq, "0") == 0 &&
             seq_valid(seq, "01") == 0 && seq_valid(seq, "012") == 0 &&
             seq_add(seq, "012") == 1)
        visited |= V(2, 1);
    else
        return visited |= V(4, 1);

    seq_delete(seq);

    return visited;
}
/*
// Sprawdza reakcję implementacji na niepowodzenie alokacji pamięci.
static int memory_test(unsigned long (* test_function)(void)) {
    memory_test_data_t *mtd = get_memory_test_data();

    unsigned fail = 0, pass = 0;
    mtd->call_total = 0;
    mtd->fail_counter = 1;
    while (fail < 3 && pass < 3) {
        mtd->call_counter = 0;
        mtd->alloc_counter = 0;
        mtd->free_counter = 0;
        mtd->function_name = NULL;
        unsigned long visited_points = test_function();
        if (mtd->alloc_counter != mtd->free_counter ||
            (visited_points & 0444444444444444444444UL) != 0) {
            fprintf(stderr,
                    "fail_counter %u, alloc_counter %u, free_counter %u, "
                    "function_name %s, visited_point %lo\n",
                    mtd->fail_counter, mtd->alloc_counter, mtd->free_counter,
                    mtd->function_name, visited_points);
            ++fail;
        }
        if (mtd->function_name == NULL)
            ++pass;
        else
            pass = 0;

        mtd->fail_counter++;
    }
    printf("mtd -> call_total=%u call_counter=%u fail_counter=%u fail=%u\n",mtd -> call_total,mtd->call_counter, mtd->fail_counter, fail);

    return mtd->call_total > 0 && fail == 0 ? PASS : FAIL;
}

static int memory(void) {
    return memory_test(alloc_fail_seq_new_seq_add);
}
*/
typedef struct {
    char const *name;
    int (*function)(void);
} test_list_t;

#define TEST(t) {#t, t}

static const test_list_t test_list[] = {
        TEST(params),
        TEST(simple),
        TEST(equivalence),
  //      TEST(memory),
};

static int do_test(int (*function)(void)) {
    int result = function();
    puts(get_magic_string());
    return result;
}

int main(int argc, char *argv[]) {
    /*if (argc == 2)
        for (size_t i = 0; i < SIZE(test_list); ++i)
            if (strcmp(argv[1], test_list[i].name) == 0)
                return do_test(test_list[i].function);

    fprintf(stderr, "Użycie:\n%s nazwa_testu\n", argv[0]);
    return WRONG_TEST;*/
    printf("----------------------------------------------------------------------------------Params: %d\n",params());
    printf("----------------------------------------------------------------------------------Simple: %d\n",simple());
    printf("-----------------------------------------------------------------------------Equivalence: %d\n",equivalence());
    //printf("----------------------------------------------------------------------------------Custom: %d\n",custom());
    return 0;
}
///////////////////////REPLACE STRDUPS WITH STRNDUPS???????????????????? MOST LIKELY BETTER YES!!!


/*
typedef struct subset {
    int parent;
    int rank;
} subset;

static int findWithPathComp0(int i, subset *subsets[]) {
    if (subsets[i]->parent == i) return i;
    subsets[i]->parent = findWithPathComp0(subsets[i]->parent, subsets);
    return subsets[i]->parent;
}

static void unionByRank0(int a, int b, subset *subsets[]) {
    if (subsets[a]->rank < subsets[b]->rank) subsets[a]->parent = b;
    else if (subsets[a]->rank > subsets[b]->rank) subsets[b]->parent = a;
    else {
        subsets[b]->parent = a;
        subsets[a]->rank++;
    }
}*/