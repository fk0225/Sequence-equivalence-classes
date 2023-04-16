#include <stdio.h>
#include "seq.h"

int main() {
    seq_t *p = seq_new();
    seq_add(p, "012");
    seq_add(p, "0122");
    //printf("%d\n",seq_remove(p, "012"));
    seq_set_name(p,"012","seq0");
    seq_set_name(p,"0122","seq1");
    printf("%d\n",seq_valid(p, "01222"));
    printf("%d")
    //p->classNames->currCapacity;
    //printf("Size=%d\n",);
    //printf("%d\n",seq_equiv(p,"012","0122"));
    //puts(seq_get_name(p,"0122"));
    seq_add(p,"120");
    //printf("%d\n",seq_equiv(p,"012","120"));*/
    seq_delete(p);
    return 0;
}
