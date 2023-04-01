#ifndef SEQ_H
#define SEQ_H

typedef struct seq seq_t;

seq_t *      seq_new(void);
void         seq_delete(seq_t *p);
int          seq_add(seq_t *p, char const *s);
int          seq_remove(seq_t *p, char const *s);
int          seq_valid(seq_t *p, char const *s);
int          seq_set_name(seq_t *p, char const *s, char const *n);
char const * seq_get_name(seq_t *p, char const *s);
int          seq_equiv(seq_t *p, char const *s1, char const *s2);

#endif