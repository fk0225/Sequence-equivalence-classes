//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsf.h"
#include "err.h"

typedef struct UFNode {
    struct UFNode *parent;
    int size;
    //int removedChildrenCount;
    char *className;
    //int rank; ///OR EVEN REMOVE RANK ALTOGETHER AND JUSt UNION BY SIZE: RETHINK
    int sequenceDeleted;///switch to char?
} UFNode;

/**
 * Utility function responsible for encapuslating the creation of a new UFNode record.
 */
UFNode *newUFNode(char *name) {
    UFNode *newNode = (UFNode *) malloc(sizeof(UFNode));
   // printf("ALLOCATING UFNode at address %d\n", newNode);
    if (newNode == NULL) {
        memoryFailure();
        return NULL;
    }
    newNode->className = name;
    newNode->parent = newNode;
    newNode->size = 1;
    //newNode->rank = 1;
    newNode->sequenceDeleted = 0;//=newNode->removedChildrenCount=0;
    return newNode;
}

/**
 * Utility procedure responsible for encapuslating the safe deletion of a UFNode record.
 * If a node has any children, its deletion is unsafe and the procedure finishes.
 * Else it first checks if the node has a parent and if so, decreases its size field and recursively runs the procedure
 * on the parent. After the check, it deallocates the node.
 * The algorithm manages memory correctly (in term of both safety and preventing leaks) due to the following reasons:
 * 1. It trivially never deletes a potentially unsafe node.
 * 2. Due to the fact that the find operation does not use the path compression technique, once a node gains a child,
 * it never loses it. The only way for a node's size to decrease is when its children are deallocated and for every
 * node, once the procedure is launched on its last child, it deletes it (and, inductively, the same happens to its
 * chain of parents). THe only case of parents not being deleted is when the parents are still attached to the trie,
 * which is trivially handeld by further deallocations in the trie, untill all node are deleted.
 * This finally proves that the algorithm is correct.
 */
void safeDeleteUFNode(UFNode *node) {
    //  printf("Current UFNode at address %d, size=%d\n",node,node->size);
    if (node->size > 1) return;///RECHECK
    if (node->className != NULL) free(node->className);
    UFNode *parent = node->parent;///!!!!
    if (parent != node) {
        /* int parentSizeDifference=node->size+node->removedChildrenCount;
         parent->size-=parentSizeDifference;
         parent->removedChildrenCount+=parentSizeDifference;*/
        UFNode *parentUtil = parent;
       // int it = 0;
        // printf("WHILE ITERATION %d\n",it);it++;
        //  printf("BEFORE DECR: parentUtil=%d node=%d parentUtil->size=%d\n",parentUtil, node,parentUtil->size);
        parentUtil->size--;
        //  printf("AFTER DECR: parentUtil=%d node=%d parentUtil->size=%d\n",parentUtil, node,parentUtil->size);
        while (parentUtil != parentUtil->parent) {
            //    printf("WHILE ITERATION %d\n",it);it++;
            parentUtil = parentUtil->parent;
            //     printf("BEFORE DECR: parentUtil=%d node=%d parentUtil->size=%d\n",parentUtil, node,parentUtil->size);
            parentUtil->size--;
            //    printf("AFTER DECR: parentUtil=%d node=%d parentUtil->size=%d\n",parentUtil, node,parentUtil->size);
        }
        //printf("parent=%d parent->sequenceDeleted=%d parent->size=%d\n", parent, parent->sequenceDeleted, parent->size);///!!!
        if (parent->sequenceDeleted && parent->size < 2) safeDeleteUFNode(parent);///!!!!!
        //else ///RECHECK
    }
   // printf("DEALLOCATING UFNode at address %d\n", node);
    free(node);
    node = NULL;
}///POTENTIALLY SOURCE OF EXTREME MEMORY LEAKS!!!! OR EVEN ILLEGAL FREES AND CORRUPTION

/**
 * Function (or in terms of OOP: getter method) that extracts the class name from the node.
 * As specified in the task description, in case of
 */
char const *getRepresentativeClassName(UFNode *node) {
    UFNode *representative = find(node);
    if (representative->className == NULL) return nullFailure(); ///!!!
    return representative->className;
}

/**
 * Function (or in terms of OOP: setter method) that sets the name stored at the UFNode to the provided name n.
 */
int setRepresentativeClassName(UFNode *node, char const *n) {
    int nLen = strlen(n), classNameLen = 0;
    char *newName = strdup(n); ///STRNDUP!!!!!!
    if (newName == NULL) return memoryFailure();
    UFNode *representative = find(node);
    if (stringNotNullOrEmpty(representative->className)) classNameLen = strlen(representative->className);
    if (nLen == classNameLen && strncmp(n, representative->className, nLen) == 0) {
        free(newName);
        return 0;
    }
    if (representative->className != NULL) free(representative->className);
    ///ASS: n NO NULL!!!
    representative->className = newName;///!!!
    return 1;
}

/**
 * Function (or in terms of OOP: setter method) that sets the sequenceDeleted field of the UFNode to 1, hence marking
 * that is not pointed to by any trie node
 */

void setSequenceDeleted(UFNode *node) {
    node->sequenceDeleted = 1;
   //printf("!!!!!!DELETING SEQUENCE!!!!!!node=%d node->sequenceDeleted=%d\n", node, node->sequenceDeleted);
}

/**
 * Function implementing the find with path compression algorithmic technique.
 * As a side effect, it decrements the size of subtree rooted in the initial parent and increments that of its new parent
 * (in case the parent doesn't changes, the decrementation and incrementation simply cancel each other out).
 */
/*UFNode *findWithPathComp(UFNode *node) {
    if (node->parent == node) return node;
    node->parent->size--;///RECHECK
    node->parent = findWithPathComp(node->parent);
    node->parent->size++;///RECHECK
    return node->parent;
}///PARENT SIZE UPDATING MAKE SURE INCREMENTATIONS AND DECREMENTATIONS OK!!!*/

/**
 * Function implementing the find operation of the Disjoint Set Family Data structure.
 */

UFNode *find(UFNode *node) {
    while (node->parent != node) node = node->parent;
    return node;
}///PARENT SIZE UPDATING MAKE SURE INCREMENTATIONS AND DECREMENTATIONS OK!!!

/**
 * Function that implements the rules of giving new abstraction calsses names according to the rules outlined in the
 * task description.
 */

static int newName(char *a, char *b, char **new) {
    //printf("a==NULL=%d b==NULL=%d\n",a==NULL,b==NULL);
    if (a == NULL && b == NULL) {
        (*new) = NULL;
        return 1;
    } else if (a == NULL) {
        (*new) = strdup(b);///ENSURE STRDUP SAFE AND ACTUALLY DOES WHAT IT SEEMS TO
    } else if (b == NULL) {
        (*new) = strdup(a);///ENSURE MALLOC FAILURES (STRDUP RETURNING NULL) PROPERLY HANDLED!!!
    } else {
        char *aDuplicate = (char *) calloc(sizeof(char), strlen(a) + strlen(b) + 1);///recheck if +1!!!
        if (aDuplicate == NULL) return memoryFailure();
        strcpy(aDuplicate, a);
        //if (aDuplicate == NULL) return memoryFailure();
        a = strcat(aDuplicate, b);
        (*new) = a;/// RECHECK SECOND DUP NOT NEEDED!!!
        ///!!!
    }
    if ((*new) == NULL) return memoryFailure();
    return 1;
}

/**
 * Function implementing the union operation of the Disjoint Set Family, using the union by size algorithmic technique.
 */

int unionBySize(UFNode *a, UFNode *b) {
    a = find(a);
    b = find(b);
    if (a == b) return 0;
    char *aClassName = NULL, *bClassName = NULL, *newClassName = NULL;
    if (a->className != NULL) aClassName = strdup(a->className);
    if (b->className != NULL) bClassName = strdup(b->className);
    if ((a->className != NULL && aClassName == NULL) || (b->className != NULL && bClassName == NULL) ||
        newName(aClassName, bClassName, &newClassName) == -1) {
        free(aClassName);
        free(bClassName);
        free(newClassName);
        return memoryFailure();
    }
    if (a->className != NULL) free(a->className);
    if (b->className != NULL) free(b->className);
    a->className = b->className = NULL;
    if (a->size < b->size) {
        a->parent = b;
        //   printf("BEFORE UNION: b=%d b->size=%d\n",b,b->size);
        b->size += a->size;///RECHECK
        //   printf("BEFORE UNION: b=%d b->size=%d\n",b,b->size);
        b->className = newClassName;
    } /*else if (a->size > b->size) {
        b->parent = a;
        //printf("BEFORE UNION: a=%d b=%d a->size=%d\n",a,a->size);
        a->size += b->size;///RECHECK
        // printf("AFTER UNION: a=%d b=%d a->size=%d\n",a,a->size);
        a->className = newClassName;
    } */else {
        b->parent = a;
        // a->rank++;
        //  printf("BEFORE UNION: a=%d b=%d a->size=%d\n",a,b,a->size);
        a->size += b->size;///RECHECK
        //  printf("AFTER UNION: a=%d b=%d a->size=%d\n",a,b,a->size);
        a->className = newClassName;
    }
    free(aClassName);
    free(bClassName);
    return 1;
}///UPDATING RANKS???
/*
void printClassName(UFNode *node) {
    if (node->className == NULL) puts("NULL");
    else puts(node->className);
}

void printNodeRank(UFNode *node) {
    printf("%d\n", node->rank);
}*/