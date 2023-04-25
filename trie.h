#ifndef TRIE_H
#define TRIE_H

/**
 * trieNode data record, implementing the Trie data structure.
 */
typedef struct trieNode node;

/**
 * @return a newly allocated trieNode record.
 */
node *newTrieNode(void);

/**
 * @param root: root of the trie tree being deleted.
 */
void deleteTrie(node *root);

/**
 * @param t: trieNode record whose class name is being extracted.
 * @return the record's class name.
 */
char const* getClassName(node* t); ///CHAR CONST!!!

/**
 * @param t
 * @param n
 * @return
 */
int setClassName(node* t, char const* n);

node *searchTrieNode(node *root, /*int* result,*/ char const *s, int sLen);

int insertSequence(node *root, /*int* result,*/ char const *s, int sLen);

int removeSequence(node *root, /*int* result,*/ char const *s, int sLen);

int searchSequence(node *root, /*int* result,*/ char const *s, int sLen);

int mergeClasses(node* node1, node* node2);

#endif
