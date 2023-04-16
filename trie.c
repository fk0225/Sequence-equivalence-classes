#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "trie.h"

node *newTrieNode(void) {
    node *newNode = (node *) malloc(sizeof(node));
    if (newNode == NULL) {
        memoryFailure();
        return NULL;///RECHECK IF ACCORDS WITH THE SPECIFICATION!!!
    }
    newNode->classIndex = -1;
    /*newNode->classNameNode=newUFNode(NULL);
    if(newNode->classNameNode==NULL) {
        errno=ENOMEM;
        return NULL;
    }*/
    for (int i = 0; i < 3; i++) newNode->children[i] = NULL;
    return newNode;
} ///MEMORY FAILURES AND ERRORS MANAGMENT!!!!!!!!!!!!!!

void deleteTrie(node *root) {
    for (int i = 0; i < 3; i++) {
        if (root->children[i] != NULL) {
            deleteTrie(root->children[i]);
            root->children[i] = NULL;
        }
    }
    //free(root->classNameNode);
    free(root);
}

static char correspondingInt(int i) {
    return i - '0';
}

node *searchTrieNode(node *root, /*int* result,*/ char const *s, int sLen) {
    int i = 0;
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        root = root->children[correspondingInt(s[i++])];
    }
    if (i == sLen) return root;
    return NULL;
} ///SOEHOW FUNCTIONALLY REPLACE ALL OTHER SUCH SEARCHES IN INSERITON, REMOVAL ETC WITH THIS UTILITY


/*node**/ int insertSequence(node *root, /*int* result,*/ char const *s, int sLen) {
    int i = 0, memoryFailed=0;
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        root = root->children[correspondingInt(s[i++])];
    }
    if (i == sLen) {
        //*result=0;

        return 0;
    }
    int iPrev=i;
    node * lastNotNull=root;
    while (i < sLen) {
        root = root->children[correspondingInt(s[i++])] = newTrieNode();
        if(root==NULL) {
            memoryFailed=1;
            break;
        }
    }
    if(memoryFailed) {
        deleteTrie(lastNotNull->children[correspondingInt(s[iPrev])]);
    }
    //*result=1;
    ///memory management, errors, and other reasons for -1
    if(memoryFailed) return memoryFailure();
    return 1;
}

/*node**/ int removeSequence(node *root, /*int* result,*/ char const *s, int sLen) {
    int i = 0;
    sLen--;
    while (i < sLen && root->children[correspondingInt(s[i])] != NULL) {
        root = root->children[correspondingInt(s[i++])];
    }
    if (i < sLen) {
        return 0;
    }
    deleteTrie(root); ///recheck if here exactly deletion begins but most likely
    root = NULL;
    //*result=1;
    return 1;
    ///when exactly -1??!!
}

int searchSequence(node *root, /*int* result,*/ char const *s, int sLen) {
    ///MEMORY MANAGEMENT AND POTENTIAL ERRORS!!!
    return searchTrieNode(root, s, sLen) != NULL;
}

///MOST LIKELY MODULE EITHER UNNECESSARY OR NEEDS TO BE REORGANIZED!!!