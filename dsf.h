#ifndef DSF_H
#define DSF_H

/**
 * UFNode data record, implementing the Disjoint Set Family data structure.
 */
typedef struct UFNode UFNode;

/**
 * @param name: the name of the class stored by the new node (potentially NULL).
 * @return the newly allocated record.
 */
UFNode *newUFNode(char *name);

/**
 * @param node: UFNode record scheduled for deletion.
 */
void safeDeleteUFNode(UFNode *node);


/**
 * @param node: UFNode record whose source trieNode record corresponds to a sequence scheduled for deletion.
 */
void setSequenceDeleted(UFNode *node);


/**
 * @param node: node whose representative's class name is being extracted.
 * @return the representative's class name.
 */
char const *getRepresentativeClassName(UFNode *node);

/**
 * @param node: a node of the UFNode record, whose representative will be modified.
 * @param n: new name of the representative of node.
 * @return exit code according to the specification of the task.
 */
int setRepresentativeClassName(UFNode *node, char const *n);

/**
 * @param node: a node of the UFNode record, whose representative will be found.
 * @return the representative of node.
 */
UFNode *find(UFNode *node);


/**
 * @param a: first node (within some disjoint subset).
 * @param b : second node (within some disjoint subset, assumed to be different that that of a).
 * @return exit code according to the specification of the task.
 */
int unionBySize(UFNode *a, UFNode *b);

#endif
