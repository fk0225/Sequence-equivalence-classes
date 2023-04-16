#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 3

typedef struct trieNode {
    int classIndex;
    //UFNode* classNameNode;
    ///BEFORE TRYING TO ACCESS THE CLSS NAME FIND THE NODES PRENT WITH PATH COMPRESSION (AND THEN GIVE THE PARENT'S CLASS NAME!!!!)
    struct trieNode *children[ALPHABET_SIZE];///<Tablica dzieci indeksowana literami alfabetu (w tym przypadku cyframi 0-11)
} node;

node *newTrieNode(void);

void deleteTrie(node *root);

node *searchTrieNode(node *root, /*int* result,*/ char const *s, int sLen);

int insertSequence(node *root, /*int* result,*/ char const *s, int sLen);

int removeSequence(node *root, /*int* result,*/ char const *s, int sLen);

int searchSequence(node *root, /*int* result,*/ char const *s, int sLen);

#endif
