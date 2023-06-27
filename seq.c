
#include <stdlib.h>
#include <string.h>
#include "seq.h"
#include "err.h"
#include "trie.h"

/**
 * Implementation of the seq data record, consisting of a pointer to a record defined in the trie.h header.
 */
typedef struct seq {
    node *trieRoot;
} seq_t;


/**
 * Implementation of the seq_new function/method, according to the specification of the task.
 */
seq_t *seq_new(void) {
    seq_t *newSeq = (seq_t *) malloc(sizeof(seq_t));
    if (newSeq == NULL) {
        memoryFailure();
        return NULL;
    }
    newSeq->trieRoot = newTrieNode();
    if (newSeq->trieRoot == NULL) {
        memoryFailure();
        free(newSeq);
        return NULL;
    }
    return newSeq;
}

/**
 * Implementation of the seq_delete function/method, according to the specification of the task.
 */
void seq_delete(seq_t *p) {
    if (p == NULL) return;
    deleteTrie(p->trieRoot);
    free(p);
    p = NULL;
}

/**
 * Implementation of the seq_add function/method, according to the specification of the task.
 */
int seq_add(seq_t *p, char const *s) {
    if (!(p != NULL && stringNotNullOrEmpty(s))) return invalidArg();
    long long unsigned sLen = strlen(s);
    if (!stringAllowedAs012Seq(s, sLen)) return invalidArg();
    return insertSequence(p->trieRoot, s, sLen);
}

/**
 * Implementation of the seq_remove function/method, according to the specification of the task.
 */
int seq_remove(seq_t *p, char const *s) {
    if (!(p != NULL && stringNotNullOrEmpty(s))) return invalidArg();
    long long unsigned sLen = strlen(s);
    if (!stringAllowedAs012Seq(s, sLen)) return invalidArg();
    int res = removeSequence(p->trieRoot, s, sLen);
    return res;
}

/**
 * Implementation of the seq_valid function/method, according to the specification of the task.
 */
int seq_valid(seq_t *p, char const *s) {
    if (!(p != NULL && stringNotNullOrEmpty(s))) return invalidArg();
    long long unsigned sLen = strlen(s);
    if (!stringAllowedAs012Seq(s, sLen)) return invalidArg();
    return searchSequence(p->trieRoot, s, sLen);
}

/**
 * Implementation of the seq_set_name function/method, according to the specification of the task.
 */
int seq_set_name(seq_t *p, char const *s, char const *n) {
    if (!(p != NULL && stringNotNullOrEmpty(s) && stringNotNullOrEmpty(n))) return invalidArg();
    long long unsigned sLen = strlen(s);
    if (!stringAllowedAs012Seq(s, sLen)) return invalidArg();
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    if (t == NULL) return 0;
    return setClassName(t, n);
}

/**
 * Implementation of the seq_get_name function/method, according to the specification of the task.
 */
char const *seq_get_name(seq_t *p, char const *s) {
    if (!(p != NULL && stringNotNullOrEmpty(s))) {
        invalidArg();
        return NULL;
    }
    long long unsigned sLen = strlen(s);
    if (!stringAllowedAs012Seq(s, sLen)) {
        invalidArg();
        return NULL;
    }
    node *t = searchTrieNode(p->trieRoot, s, sLen);
    if (t == NULL) return nullFailure();
    return getClassName(t);
}

/**
 * Implementation of the seq_equiv function/method, according to the specification of the task.
 */
int seq_equiv(seq_t *p, char const *s1, char const *s2) {
    if (!(p != NULL && stringNotNullOrEmpty(s1) && stringNotNullOrEmpty(s2))) return invalidArg();
    node *root = p->trieRoot;
    long long unsigned s1Len = strlen(s1), s2Len = strlen(s2);
    if (!(stringAllowedAs012Seq(s1, s1Len) && stringAllowedAs012Seq(s2, s2Len))) return invalidArg();
    if (s1Len == s2Len && strncmp(s1, s2, s1Len) == 0) return 0;
    node *node1 = searchTrieNode(root, s1, s1Len), *node2 = searchTrieNode(root, s2, s2Len);
    return mergeClasses(node1, node2);
}
