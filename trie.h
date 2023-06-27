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
char const *getClassName(node *t);

/**
 * @param t: trieNode record whose class name is being set.
 * @param n: the new name given to the class.
 * @return exit code specified in the task description.
 */
int setClassName(node *t, char const *n);

/**
 * @param root: the root of the trie being searched.
 * @param s: the sequence whose corresponding node is being searched.
 * @param sLen: the length of s.
 * @return exit code specified in the task description.
 */
node *searchTrieNode(node *root, char const *s, long long unsigned sLen);

/**
 * @param root
 * @param s: the sequence whose corresponding node is being searched.
 * @param sLen: the length of s.
 * @return exit code specified in the task description.
 */
int insertSequence(node *root, char const *s, long long unsigned sLen);

/**
 * @param root: the root of the trie being removed.
 * @param s: the sequence whose corresponding node is being searched.
 * @param sLen: the length of s.
 * @return exit code specified in the task description.
 */
int removeSequence(node *root, char const *s, long long unsigned sLen);

/**
 * @param root: the root of the trie being searched.
 * @param s: the sequence whose corresponding node is being searched.
 * @param sLen: the length of s.
 * @return exit code specified in the task description.
 */
int searchSequence(node *root, char const *s, long long unsigned sLen);

/**
 * @param node1: the first node whose class is being merged.
 * @param node2: the second node whose class is being merged.
 * @return exit code specified in the task description.
 */
int mergeClasses(node *node1, node *node2);

#endif
