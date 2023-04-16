#ifndef DSF_H
#define DSF_H

typedef struct UFNode UFNode;

UFNode * newUFNode(char* name);

UFNode *findWithPathComp(UFNode *node);

int unionByRank(UFNode *a, UFNode *b);

#endif
