
/**
 * Implementation of the UFNode record (or in accordance with OOP terminology: class) with its utility functions (or,
 * in terms of OOP, class methods), implementing the functionalities of the Disjoint Set Family data structure.
 */

#include <stdlib.h>
#include <string.h>
#include "dsf.h"
#include "err.h"

/**
 * Data record based on the Disjoint set family data structure, with pointers to the record's parent as well as the name
 * of the class the record stores. It also contains two int fields size and sequenceDeleted, for the purpose of handling
 * safe deletion of the record.
 */
typedef struct UFNode {
    struct UFNode *parent;
    int size;
    char *className;
    int sequenceDeleted;
} UFNode;

/**
 * Utility function responsible for encapuslating the creation of a new UFNode record.
 */
UFNode *newUFNode(char *name) {
    UFNode *newNode = (UFNode *) malloc(sizeof(UFNode));
    if (newNode == NULL) {
        memoryFailure();
        return NULL;
    }
    newNode->className = name;
    newNode->parent = newNode;
    newNode->size = 1;
    newNode->sequenceDeleted = 0;
    return newNode;
}

/**
 * Utility procedure responsible for encapuslating the safe deletion of a UFNode record.
 *
 * If a node has any children, its deletion is unsafe and the procedure finishes.
 * Else it first checks if the node has a parent and if so, decreases the value of the size field of all of its
 * ancestors and recursively runs the procedure on the parent. After the check, it deallocates the node.
 *
 * The algorithm manages memory correctly (in term of both safety and preventing leaks) due to the following reasons:
 *
 * 1. It trivially never deletes a potentially unsafe node.
 * 2. Due to the fact that the find operation does not use the path compression technique, once a node gains a child,
 * it never loses it.
 *
 * The only way for a node's size to decrease is when its children are deallocated and for every
 * node, once the procedure is launched on its last child, it deletes it (and, inductively, the same happens to all of
 * its ancestors). The only case of parents not being deleted is when the parents are still attached to the trie,
 * which is trivially handled by further deallocations in the trie, until all node have been deleted.
 *
 * This finally proves that the algorithm is correct.
 */
void safeDeleteUFNode(UFNode *node) {
    if (node->size > 1) return;
    if (node->className != NULL) free(node->className);
    UFNode *parent = node->parent;
    if (parent != node) {
        UFNode *parentUtil = parent;
        parentUtil->size--;
        while (parentUtil != parentUtil->parent) {
            parentUtil = parentUtil->parent;
            parentUtil->size--;
        }
        if (parent->sequenceDeleted && parent->size < 2) safeDeleteUFNode(parent);
    }
    free(node);
    node = NULL;
}

/**
 * Function (or in terms of OOP: getter method) that extracts the class name from the node.
 * As specified in the task description, in case of the class name not being set, returns null failure.
 */
char const *getRepresentativeClassName(UFNode *node) {
    UFNode *representative = find(node);
    if (representative->className == NULL) return nullFailure();
    return representative->className;
}

/**
 * Function (or in terms of OOP: setter method) that sets the name stored at the UFNode to the provided name n.
 */
int setRepresentativeClassName(UFNode *node, char const *n) {
    long long unsigned nLen = strlen(n), classNameLen = 0;
    char *newName = strdup(n);
    if (newName == NULL) return memoryFailure();
    UFNode *representative = find(node);
    if (stringNotNullOrEmpty(representative->className)) classNameLen = strlen(representative->className);
    if (nLen == classNameLen && strncmp(n, representative->className, nLen) == 0) {
        free(newName);
        return 0;
    }
    if (representative->className != NULL) free(representative->className);
    representative->className = newName;
    return 1;
}

/**
 * Function (or in terms of OOP: setter method) that sets the sequenceDeleted field of the UFNode to 1, hence marking
 * that is not pointed to by any trie node.
 */

void setSequenceDeleted(UFNode *node) {
    node->sequenceDeleted = 1;
}

/**
 * Function implementing the find operation of the Disjoint Set Family Data structure.
 */

UFNode *find(UFNode *node) {
    while (node->parent != node) node = node->parent;
    return node;
}

/**
 * Function that implements the rules of giving new abstraction classes names, according to the rules outlined in the
 * task description.
 */

static int newName(char *a, char *b, char **new) {
    if (a == NULL && b == NULL) {
        (*new) = NULL;
        return 1;
    } else if (a == NULL) {
        (*new) = strdup(b);
    } else if (b == NULL) {
        (*new) = strdup(a);
    } else {
        char *aDuplicate = (char *) calloc(sizeof(char), strlen(a) + strlen(b) + 1);
        if (aDuplicate == NULL) return memoryFailure();
        strcpy(aDuplicate, a);
        a = strcat(aDuplicate, b);
        (*new) = a;
    }
    if ((*new) == NULL) return memoryFailure();
    return 1;
}

/**
 * Function implementing the union operation of the Disjoint Set Family, using the union by size algorithmic technique
 * (adding the node with a smaller subtree as a child of the one with the bigger one).
 *
 * It also handles the issue of giving new classes names (by calling the newName function) and handling potential errors
 * emerging from the process.
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
        b->size += a->size;
        b->className = newClassName;
    } else {
        b->parent = a;
        a->size += b->size;
        a->className = newClassName;
    }
    free(aClassName);
    free(bClassName);
    return 1;
}
