//#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "trie.h"
#include "dsf.h"

#define ALPHABET_SIZE 3


/**
 * Data record based on the trie data structure, with an extra pointer to another record of type UFNode responsible for
 * handling operations on the sequence's abstraction class.
 * It lacks the traditional isLeaf boolean field due to the fact that according to the specification of the task, all
 * prefixes of an added sequence also belong to the set and hence the field is redundant.
 */
typedef struct trieNode {
    UFNode* classNameNode;
    ///BEFORE TRYING TO ACCESS THE CLSS NAME FIND THE NODES PRENT WITH PATH COMPRESSION (AND THEN GIVE THE PARENT'S CLASS NAME!!!!)
    struct trieNode *children[ALPHABET_SIZE];
} node;

/**
 * Utility function responsible for encapuslating the creation of a new trieNode record.
 */
node *newTrieNode(void) {
    node *newNode = (node *) malloc(sizeof(node));
    if (newNode == NULL) {
        memoryFailure();
        return NULL;///RECHECK IF ACCORDS WITH THE SPECIFICATION!!!
    }
    //newNode->classIndex = -1;
    newNode->classNameNode=newUFNode(NULL);
    if(newNode->classNameNode==NULL) {
        memoryFailure();
        free(newNode);
        return NULL;
    }
    for (int i = 0; i < 3; i++) newNode->children[i] = NULL;
    return newNode;
} ///MEMORY FAILURES AND ERRORS MANAGMENT!!!!!!!!!!!!!!

/**
 * Utility function responsible for encapsulating the deletion of a trieNode record.
 */
void deleteTrie(node *root) {
    if(root==NULL) return;
    for (int i = 0; i < 3; i++) {
        //if (root->children[i] != NULL) {
            deleteTrie(root->children[i]);
            root->children[i] = NULL;
        //}
    }
    setSequenceDeleted(root->classNameNode);
    safeDeleteUFNode(root->classNameNode);
    free(root);
}

/**
 * Function (or in terms of OOP: getter method) that extracts the class name from the Union Find node responsible for
 * storing information about it, by calling its own appropriate getter "method".
 */
char const* getClassName(node* t) {
    return getRepresentativeClassName(t->classNameNode);
}

/**
 * Function (or in terms of OOP: setter method) that sets the class name from the Union Find node responsible for
 * storing information about it, by calling its own appropriate setter "method".
 */

int setClassName(node* t, char const* n) {
    return setRepresentativeClassName(t->classNameNode,n);
}

/**
 * Utility function for converting a character to the integer it represents.
 */

static int correspondingInt(char i) {
    return i - '0';
}

/**
 * Function that goes down the trie until it has traversed through the whole string s, or has encountered NULL.
 * In the former case returns a pointer to the last encountered node, in the latter returns NULL.
 */

node *searchTrieNode(node *root, /*int* result,*/ char const *s, int sLen) {
    int i = 0;
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        //printf("next letter: %c\n",s[i]);
        root = root->children[correspondingInt(s[i++])];
    }
    //printf("s[%d]=%c\n",i,s[i]);
    if (i == sLen) return root;
    return NULL;
} ///SOEHOW FUNCTIONALLY REPLACE ALL OTHER SUCH SEARCHES IN INSERTION, REMOVAL ETC WITH THIS UTILITY

/**
 * Function responsible for adding the sequence (and all of its prefixes) to the trie.
 * It is based on going down the trie by characters of the new sequence until either the string ends (in this case the
 * function returns 0) or until a leaf node is encountered and there are still characters left (in this case, a new node
 * inserted, in the same way as previously going down the tree , until all characters are inserted.
 * For the case of encountering an allocation failure, the function holds an extra index of the last character in the
 * string already in the trie, so that once a failure is encountered, one can deallocate the subtree so far added and
 * safely return 0 in a state where the trie is unmodified compared to the state prior to the function's execution.
 */

/*node**/ int insertSequence(node *root, /*int* result,*/ char const *s, int sLen) {
   // printf("s=%s\n",s);
    int i = 0;//, memoryFailed=0;
    //int xd=1;
    node * lastNotNull=root;//!!
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        lastNotNull=root;
        root = root->children[correspondingInt(s[i++])];
    }
    //printf("i=%d\n",i);
    if (i == sLen) {
        return 0;
    }
    int iLast=i;
    while (i < sLen) {
        root = root->children[correspondingInt(s[i++])] = newTrieNode();
        if(root==NULL) {
            deleteTrie(lastNotNull->children[correspondingInt(s[iLast])]);
            lastNotNull->children[correspondingInt(s[iLast])]=NULL;
            return memoryFailure();
        }
    }
  //  if(memoryFailed) {
        //setSequenceDeleted(root->classNameNode); ///!!!
    //}
    return 1;
}

/**
 * Function that removes a sequence (and all sequences it is a prefix of) from the set represented by the trie.
 * If such a sequence exists, than the subtree starting in its corresponding node is deallocated and function returns 1.
 * Else nothing happens and the function returns 0.
 */

/*node**/ int removeSequence(node *root, /*int* result,*/ char const *s, int sLen) {
    int i = 0;
    //sLen--;
    node *prev = NULL;
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        prev=root;
        root = root->children[correspondingInt(s[i++])];
    }
    if (i < sLen) {
        return 0;
    }
    setSequenceDeleted(root->classNameNode); ///!!!
    deleteTrie(root); ///recheck if here exactly deletion begins but most likely
    root = NULL;
    prev->children[correspondingInt(s[i-1])]=NULL;
    return 1;
}

/**
 * Function that returns 1 if and only if a trie node corresponding to the presence of s in the set, else 0.
 */

int searchSequence(node *root, /*int* result,*/ char const *s, int sLen) {
    ///MEMORY MANAGEMENT AND POTENTIAL ERRORS!!!
    return searchTrieNode(root, s, sLen) != NULL;
}

/**
 * Function (or "method") of the trie "class" responsible for performing equivalence, that calls the unionByRank
 * function ("method") of the UFNode "class".
 */

int mergeClasses(node* node1, node* node2) {
    if (node1 == NULL || node2 == NULL) return 0;
    return unionBySize(node1->classNameNode,node2->classNameNode);
}

///MOST LIKELY MODULE EITHER UNNECESSARY OR NEEDS TO BE REORGANIZED!!!