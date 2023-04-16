#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "seq.h"
#include "err.h"
#include "trie.h"
#include "dsf.h"

#define ALPHABET_SIZE 3

typedef struct vec_int {
    int *array;
    int currCapacity;
    int maxCapacity;
} vector_int;

static vector_int *newEmptyVector_int(void) {
    vector_int *newVector = (vector_int *) malloc(sizeof(vector_int));
    newVector->currCapacity = newVector->maxCapacity = 0;
    newVector->array = NULL;
}///RETHINK IF POINTER TO VECTOR ACTUALLY NEEDED

static void addElem_vi(vector_int *vi, int n) {
    if (vi->currCapacity == vi->maxCapacity) {
        vi->maxCapacity <<= 1;
        vi->array = (int *) realloc(vi->array, vi->maxCapacity * sizeof(int));
    }
    vi->array[vi->currCapacity] = n;
    vi->currCapacity++;
}

typedef struct vec_str {
    char **array;
    int currCapacity;
    int maxCapacity;
} vector_string;

static vector_string *newEmptyVector_string(void) {
    vector_int *newVector = (vector_int *) malloc(sizeof(vector_int));
    newVector->currCapacity = newVector->maxCapacity = 0;
    newVector->array = NULL;
}///RETHINK IF POINTER TO VECTOR ACTUALLY NEEDED

static void addElem_vs(vector_string *vs, char *s) {
    if (vs->currCapacity == vs->maxCapacity) {
        vs->maxCapacity <<= 1;
        puts("ClassNames size DOUBLED");
        vs->array = (int *) realloc(vs->array, vs->maxCapacity * sizeof(int));
        if(vs->array==NULL) {
            errno=ENOMEM;
            return;
        }
    }
    vs->array[vs->currCapacity] = s;
    vs->currCapacity++;
}

typedef struct seq {
    node *trieRoot;
    vector_int *classSizes;
    vector_string *classNames;
} seq_t;

seq_t *seq_new(void) {
    seq_t *newSeq = (seq_t *) malloc(sizeof(seq_t));
    if(newSeq==NULL) {
        memoryFailure();
        return NULL;
    }
    newSeq->trieRoot = newTrieNode();
    newSeq->classSizes = newEmptyVector_int();
    newSeq->classNames = newEmptyVector_string();
    //newSeq->classNames->currCapacity=newSeq->;
    return newSeq;
}

void seq_delete(seq_t *p) {
    if(p==NULL) return;
    deleteTrie(p->trieRoot);
    for (int i = 0; i < p->classNames->currCapacity; i++) {
        free(p->classNames->array[i]);
    }
    free(p->classNames->array);
    free(p->classSizes->array);
    free(p->classNames);
    free(p->classSizes);
    free(p);
    p = NULL;
}

int seq_add(seq_t *p, char const *s) {
    int sLen = strlen(s);//, result;
    if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return invalidArg();
    return insertSequence(p->trieRoot/*&result*/, s, sLen);
    //return result;
}

static int deleteClass(seq_t *p, int classIndex) {
    p->classSizes->array[classIndex] = -1;
    free(p->classNames->array[classIndex]);
    p->classNames->array[classIndex] = NULL;
    return 1;
}

int seq_remove(seq_t *p, char const *s) {
    int sLen = strlen(s), currClassIndex;
    if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return invalidArg();
    int res = removeSequence(p->trieRoot/*&result*/, s, sLen);
    p->classSizes->array[currClassIndex]--;
    if (p->classSizes->array[currClassIndex] == 0) {
        return deleteClass(p, currClassIndex);
    }
    //return result;
    return res;
}

int seq_valid(seq_t *p, char const *s) {
    int sLen = strlen(s);
    if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return invalidArg();
    return searchSequence(p->trieRoot, s, sLen);
}

static int addClass(seq_t *p, char *className, int nInstances) {
    addElem_vs(p->classNames, className);
    addElem_vi(p->classSizes, nInstances);
    return 1;
}

int seq_set_name(seq_t *p, char const *s, char const *n) {
    int sLen = strlen(s);//, result;
    if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return invalidArg();
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    if (t == NULL) return 0;///receck specifcation
    if (t->classIndex == -1) {
        t->classIndex = p->classNames->currCapacity;
        addClass(p, n, 1);
    } else {
        p->classNames->array[t->classIndex] = n;
    }
    return 1;
}

char const *seq_get_name(seq_t *p, char const *s) {
    int sLen = strlen(s);//, result;
    ///if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return -1;
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    printf("t=%d\n", t);
    printf("CLASSINDEX: %d\n", t->classIndex);
    if (t->classIndex == -1) return NULL;
    return p->classNames->array[t->classIndex];
}

static int updateClassIndex(node *root, int index, int newIndex) {
    if (root == NULL) return 0;
    printf("Index=%d newIndex=%d classIndex=%d\n", index, newIndex, root->classIndex);
    int res = 0;
    if (root->classIndex == index) {
        puts("IN IF");
        printf("root=%d\n", root);
        res = 1;
        printf("Before ass: classIndex=%d\n", root->classIndex);
        root->classIndex = newIndex;
        printf("After ass: classIndex=%d\n", root->classIndex);
    }
    for (int i = 0; i < 3; i++) {
        res = res || updateClassIndex(root->children[i], index, newIndex);
    }
    return res;
}////!!!!!!!!!!!!!!!!!!!!!!!!

static char const *newClassName(seq_t *p, int index1, int index2) {
    printf("index1=%d index2=%d currCapacity=%d\n", index1, index2, p->classNames->currCapacity);
    puts("ClassNames:");
    for (int i = 0; i < p->classNames->currCapacity; i++) printf("className[%d]=%s\n", i, p->classNames->array[i]);
    puts("ClassSizes:");
    for (int i = 0; i < p->classSizes->currCapacity; i++) printf("classSize[%d]=%d\n", i, p->classSizes->array[i]);
    if (index1 < 0 || p->classNames->array[index1] == NULL) return p->classNames->array[index2];
    if (index2 < 0 || p->classNames->array[index2] == NULL) return p->classNames->array[index1];
    return strcat(p->classNames->array[index1], p->classNames->array[index2]); ////REPAIR STRCAT!!!
}

static int cleanUpMergedClasses(seq_t *p, int index1, int index2, int newIndex) {
    node *root = p->trieRoot;
    updateClassIndex(root, index1, newIndex);
    updateClassIndex(root, index2, newIndex);
    //printf("ClassIndex2 after update finished: %d\n",)
    if (index1 > -1) deleteClass(p, index1);
    if (index2 > -1) deleteClass(p, index2);
    return 1;
}

int seq_equiv(seq_t *p, char const *s1, char const *s2) {
    node *root = p->trieRoot;
    int s1Len = strlen(s1), s2Len = strlen(s2);
    node *node1 = searchTrieNode(root, s1, s1Len), *node2 = searchTrieNode(root, s2, s2Len);
    if (node1 == NULL || node2 == NULL) return 0;
    int index1 = node1->classIndex, index2 = node2->classIndex, newIndex = p->classSizes->currCapacity;

    if (node1->classIndex == node2->classIndex) {
        if (node1->classIndex != -1) return 0;
        addClass(p, NULL, 2);
        return cleanUpMergedClasses(p, index1, index2, newIndex);
    }

    addClass(p, newClassName(p, index1, index2), 2);

    printf("index1=%d index2=%d currCapacity=%d\n", index1, index2, p->classNames->currCapacity);
    puts("ClassNames:");
    for (int i = 0; i < p->classNames->currCapacity; i++) printf("className[%d]=%s\n", i, p->classNames->array[i]);
    puts("ClassSizes:");
    for (int i = 0; i < p->classSizes->currCapacity; i++) printf("classSize[%d]=%d\n", i, p->classSizes->array[i]);

    return cleanUpMergedClasses(p, index1, index2, newIndex);
}
/*
int seq_set_nameUF(seq_t *p, char const *s, char const *n) {
    int sLen = strlen(s);//, result;
    if(!(p!=NULL&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return -1;
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    if (t == NULL) return 0;///receck specifcation
    char* newName=strdup(n);
    if(newName==NULL) return memoryFailure();/// 0 vs -1 CASE!!!
    UFNode* representative=findWithPathComp(t->classNameNode);
    free(representative->className);
    ///ASS: n NO NULL!!!
    representative->className=newName;
    return 1;
}

char const *seq_get_nameUF(seq_t *p, char const *s) {
    int sLen = strlen(s), result;
    if(!(seqSetNotNull(p)&&stringNotNullOrEmpty(s)&&stringAllowedAs012Seq(s,sLen))) return -1;
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    //printf("t=%d\n", t);
    return findWithPathComp(t->classNameNode)->className;///VS A STRDUP OF IT !!!!!!!!
}

int seq_equivUF(seq_t *p, char const *s1, char const *s2) {
    node *root = p->trieRoot;
    int s1Len = strlen(s1), s2Len = strlen(s2);
    if(!(p!=NULL&&stringNotNullOrEmpty(s1)&&stringAllowedAs012Seq(s1,s1Len)
        &&stringNotNullOrEmpty(s2)&&stringAllowedAs012Seq(s2,s1Len))) return -1;
    node *node1 = searchTrieNode(root, s1, s1Len), *node2 = searchTrieNode(root, s2, s2Len);
    if (node1 == NULL || node2 == NULL) return 0;
    UFNode *a=node1->classNameNode, *b=node2->classNameNode;
    return unionByRank(a,b);
}


////ADDD MAJOR ERROR PROP ASPECTS LIKE DELETING EXTRA ADDED NODES WHEN FAILURE!!!!*/