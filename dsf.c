#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsf.h"
#include "err.h"


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
}


typedef struct UFNode {
    struct UFNode *parent;
    char *className;
    int rank;
} UFNode;

UFNode * newUFNode(char* name) {
    UFNode* newNode = (UFNode*) malloc(sizeof(UFNode));
    if(newNode==NULL) {
        memoryFailure();
        return NULL;
    }
    newNode->className=name;
    newNode->parent=newNode;
    newNode->rank=1;
    return newNode;
}

UFNode *findWithPathComp(UFNode *node) {
    if (node->parent == node) return node;
    node->parent = findWithPathComp(node->parent);
    return node->parent;
}

static int newName(char* a, char* b, char** new) {
    if(a==NULL&&b==NULL) {
        (*new) = NULL;
        return 1;
    } else if(a==NULL) {
        (*new)=strdup(b);///ENSURE STRDUP SAFE AND ACTUALLY DOES WHAT IT SEEMS TO
    } else if(b==NULL) {
        (*new)=strdup(a);///ENSURE MALLOC FAILURES (STRDUP RETURNING NULL) PROPERLY HANDLED!!!
    } else {
        char* aDuplicate=strdup(a);
        if(aDuplicate==NULL) return -1;
        (*new)=strcat(aDuplicate,b);/// RECHECK SECOND DUP NOT NEEDED!!!
    }
    if((*new)==NULL) return -1;
    return 1;
}

int unionByRank(UFNode *a, UFNode *b) {
    //SOmehow remeber both strings
    a=findWithPathComp(a);///RECHECK IF GOOD IDEA BU MOST LIKELY YES
    b=findWithPathComp(b);
    char *aClassName = strdup(a->className), *bClassName = strdup(b->className), *newClassName=NULL;
    if(aClassName==NULL||bClassName==NULL) return -1;
    if(newName(aClassName,bClassName,&newClassName)==-1) return -1;
    free(a->className);
    free(b->className);

    if (a->rank < b->rank) {
        a->parent = b;
        b->className=newClassName; ///ACTUALLY NOT b classname but the classname of the ROOT OF THE WHOLE CLASS TREE!!!
        ///or just ensure a and b are ways the roots???
        //somehow ypdate mame in b and remove in a
    } else if (a->rank > b->rank) {
        b->parent = a;
        a->className=newClassName;
        //somehow ypdate mame in a and remove in b
    } else {
        b->parent = a;
        a->rank++;
        a->className=newClassName;
        //somehow ypdate mame in b and remove in a
    }
    //somehow join the names
    free(aClassName);
    free(bClassName);
    return 1;
}

