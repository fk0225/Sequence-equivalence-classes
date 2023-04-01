//#include <stdio.h>
#include "seq.h"
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 3

typedef struct vec_int {
    int* array;
    int currCapacity;
    int maxCapacity;
} vector_int;

static vector_int newVector_int(int* array, int n) {

}

static vector_int newEmptyVector_int(void) {
    //vector_int newVector=(vector_int);
}///RETHINK IF POINTER TO VECTOR ACTUALLY NEEDED

typedef struct UFNode {
    struct UFNode* parent;
    int rank;
    char* className;
} subset;

static subset* newDisjointSubset(subset* newSubsetParent, char* newSubsetClassName) {
    subset* newSubset=(subset*)malloc(sizeof(subset));
    newSubset->className=newSubsetClassName;
    newSubset->parent=newSubsetParent;
    newSubset->rank=0;
    return newSubset;
} ///later add result ointer etc to signal failures etc

static int deleteDisjointSubset (subset* s) {
    free(s->className);///POTENTIALYY DANGEROUS: RETHINK NOT HAVING COPIES!!!
    free(s);
    return 1;///???
}

typedef struct trieNode {
    int isLeaf; ///recheck if actually necessary
    //char** className;
    subset* classNameNode;
    struct trieNode *parent;///<Fastryga umozliwiajaca dostep do poprzedzajacego wezla na sciezce od korzenia
    struct trieNode *children[ALPHABET_SIZE];///<Tablica dzieci indeksowana literami alfabetu (w tym przypadku cyframi 0-11)
} node;

static node* newTrieNode(int isLeafVal) {
    node* newNode = (node*) malloc(sizeof(node));
    if(newNode==NULL) return NULL;///RECHECK IF ACCORDS WITH THE SPECIFICATION!!!
    newNode->parent=NULL;
    newNode->classNameNode=newDisjointSubset(NULL,NULL);
    for(int i=0;i<3;i++) newNode->children[i]=NULL;
    newNode->isLeaf=isLeafVal;
    return newNode;
} ///MEMORY FAILURES AND ERRORS MANAGMENT!!!!!!!!!!!!!!

static void deleteTrie(node* root) {
    for(int i=0;i<3;i++) {
        if(root->children[i]!=NULL) {
            deleteTrie(root->children[i]);
        }
    }
    /*if(*(root->classNameNode)!=NULL) free(*(root->classNameNode));
    free(root->classNameNode); ///RECKECK IF ALL NECESSARY FREED POINTER NULLIFICATIONS ARE EVERYWHERE AND CORRECT!!!!!!!!!*/
    deleteDisjointSubset(root->classNameNode);
    free(root);
    root=NULL;
}

static char correspondingChar(char c) {
    return c+'0';
}

static char correspondingInt(int i) {
    return i-'0';
}

static subset* findWithPathCompression(subset *s/*, char const** className*/) {
    if(s->parent!=NULL) {
        s->parent=findWithPathCompression(s->parent/*,className*/);
        return s->parent;
    }
    //*className=s->className;
    return s;
} ///receck correctenss ut most likely ok, ERROR PROP!!!

static int setNewNameForUnion(subset* newParent, subset* s1, subset* s2) {
    if(s1->className==NULL&&s2->className==NULL) return 1;
    if(s1->className==NULL) {
        newParent->className=s2->className;
        return 1;
    }
    if(s2->className==NULL) {
        newParent->className=s1->className;
        return 1;
    }
    if(strcmp(s1->className,s2->className)==0) return 1;////recheck!!!!

    char* newClassName=strcat(s1->className,s2->className);
    free(s1->className);
    free(s2->className);
    s1->className=s2->className=NULL;
    newParent->className=newClassName;
    return 1;
    ///how exactly shod the name be joined?? concatenated?? IN WHAT ORDER???!!! Ok, just name of 1 `concat` name of 2

    ///DEALLOCATE THE UNNECESSARY NAME BUT FIRST MAKE ABSOLUELY SIRE ONT BREAK AND IS NECESSARY!!! MAKE SURE HE CORRECT NODE NAME DELETED!!!
}

static int unionByRank(subset* s1, subset* s2) {
  //  char *s1ClassName, *s2ClassName;
    findWithPathCompression(s1/*,&s1ClassName*/);
    findWithPathCompression(s2/*,&s2ClassName*/);///RECHECK MEMORY MANAGEMENT
    if(strcmp(s1->className,s2->className)==0) return 0;///again stcmp risky!!!
    subset* newParent=NULL;
    if(s1->rank<s2->rank) {
        newParent=s1->parent=s2;
    } else if(s1->rank>s2->rank) {
        newParent=s2->parent=s1;
    } else {
        newParent=s1->parent=s2;
        s2->rank++;
    }
    ///MEMORY MANAGEMENT AND ERROR PROP
    return setNewNameForUnion(newParent,s1,s2);
    ///class name modification details accoridng by the specification
}


static node* searchTrieNode(node* root, /*int* result,*/ char const *s, int sLen) {
    int i=0;
    while(s[i]!='\0'&&root->children[correspondingInt(s[i])]!=NULL) {
        root=root->children[correspondingInt(s[i++])];
    }
    if(i==sLen) return root;
    return NULL;
} ///SOEHOW FUNCTIONALLY REPLACE ALL OTHER SUCH SEARCHES IN INSERITON, REMOVAL ETC WITH THIS UTILITY


static /*node**/ int insertSequence(node* root, /*int* result,*/ char const *s, int sLen) {
    int i=0;
    while(i<sLen&&root->children[correspondingInt(s[i])]!=NULL) {
        root=root->children[correspondingInt(s[i++])];
    }
    if(i==sLen) {
        //*result=0;
        return 0;
    }
    while(i<sLen) {
        root=root->children[correspondingInt(s[i++])]=newTrieNode(1);
    }
    //*result=1;
    ///memory management, errors, and other reasons for -1
    return 1;
}

static /*node**/ int removeSequence(node* root, /*int* result,*/ char const *s, int sLen) {
    int i=0;
    while(i<sLen&&root->children[correspondingInt(s[i])]!=NULL) {
        root=root->children[correspondingInt(s[i++])];
    }
    if(i<sLen) {
        //*result=0;
        return 0;
    }
    deleteTrie(root); ///recheck if here exactly deletion begins but most likely
    //*result=1;
    return 1;
    ///when exactly -1??!!
}

static int searchSequence(node* root, /*int* result,*/ char const *s, int sLen) {
    ///MEMORY MANAGEMENT AND POTENTIAL ERRORS!!!
    return searchTrieNode(root,s,sLen)!=NULL;
}

static char const* getClassName(node* root, int* result, char const *s, int sLen) {
    /*int i=0;
    while(s[i]!='\0'&&root->children[correspondingInt(s[i])]!=NULL) {
        root=root->children[correspondingInt(s[i++])];
    }
    if(i<sLen) {
        *result=0;
        return NULL;
    }*/
    ///MEMORY MANAGEMENT AND POTENTIAL ERRORS!!!

    ///RUN FIND WITH PARENT UPDATE!!!
    ///Or even simpler: just merege to named string nodes??
    //node* correspondingNode;
    /*return (const char *) */
    //char* className;
    //root=searchTrieNode(root,s,sLen);
    return findWithPathCompression(searchTrieNode(root,s,sLen)->classNameNode/*,&className*/)->className; ///recheck if casting or making it const char beforehand correct!!!
    //return className;
} ///modify the className field to be wraped in the element and return the result ofind

static int setParentClassName(subset* s, char* newName) {
    free(s->className);
    s->className=newName;
}

static int setClassName(node* root, /*int* result,*/ char const *s, int sLen, char const* newName) {
    int i=0;
    while(s[i]!='\0'&&root->children[correspondingInt(s[i])]!=NULL) {
        root=root->children[correspondingInt(s[i++])];
    }
    subset* UFNode=root->classNameNode;
    if(i<sLen|| strncmp(s, (const char *) UFNode->className, sLen) == 0) { ///RECHECK MEMORY MANAGEMENT!!!!
        return 0;
    }
    return setParentClassName(UFNode,(char *) strdup(newName));

   /// *(root->className) = (char *) strdup(s); ///RECHECK IF SHOULD HAVE EFFECT ON ALL MEMBERS OF THE CLASS!!!
    ///reckeck memory management, STR FUNCTION APPROPRIATE FOR THIS AND WHY **!!!!
    ///MEMORY MANAGEMENT AND POTENTIAL ERRORS!!!

    ////ALSO ADD TO AND IF NECESSARY REMOVE FROM APPROPRIATE BTS/UF/OTHER SET!!!!!

    ///return 1;///recheck if casting or making it const char beforehand correct!!!
} ///RECHECK: AT EACH SETING CREATE NEW SINGLETON CLASS???

typedef struct seq {
    node* trieRoot;
    /**
     * Each abstraction class contained in a single set -> may be represented as array of trees/others
     * within the seq_t itself
     * UNION FIND????!!!!
     *
     */
} seq_t;

seq_t * seq_new(void) {
    seq_t* newSeq = (seq_t*) malloc(sizeof(seq_t));
    newSeq->trieRoot=newTrieNode(0);
    return newSeq;
}

void seq_delete(seq_t *p) {
    deleteTrie(p->trieRoot);
    free(p);
    p=NULL;
}

int seq_add(seq_t *p, char const *s) {
    int sLen=strlen(s), result;
    return insertSequence(p->trieRoot/*&result*/,s,sLen);
    //return result;
}

int seq_remove(seq_t *p, char const *s) {
    int sLen=strlen(s);
    return removeSequence(p->trieRoot/*&result*/,s,sLen);
    //return result;
}

int seq_valid(seq_t *p, char const *s) {
    int sLen=strlen(s);
    return searchSequence(p->trieRoot,s,sLen);
}

int seq_set_name(seq_t *p, char const *s, char const *n) {
    int sLen=strlen(s)/*, result*/;
    return setClassName(p->trieRoot,s,sLen,n); ///consider (style vs convenience and utility) switching to int and not result pointer
    //return result;
}

char const * seq_get_name(seq_t *p, char const *s) {
    int sLen=strlen(s), result;
    return getClassName(p->trieRoot,&result,s,sLen);
}

int seq_equiv(seq_t *p, char const *s1, char const *s2) {
    //subset *a=getUFNode(), *b=getUFNode();
    //subset *c=unionByRank(a,b);
    node* root=p->trieRoot;
    int s1Len=strlen(s1), s2Len=strlen(s2);
    node *node1=searchTrieNode(root,s1,s1Len), *node2=searchTrieNode(root,s2,s2Len);
    subset* c=unionByRank(node1->classNameNode,node2->classNameNode);
    node1->classNameNode=node2->classNameNode=c;///CAREFULLY RETHINK AND RECHECK
    return 1;
}

///ITS BASICALLY REPLAING FORWARD

///ALL SEQUENCES OF A PARTICULAR CLASS SHARE A POINTER TO ITS NAME IN THE UF TREE PARETNT NODE

///Idea: a bst of abstraction class names later joined using union-find???

///SEPARATE BST (or something else??) TO STORE ABSTRACTION CLASSES!!!! SI THAT QUICKLY JOINING POSSIBLE????