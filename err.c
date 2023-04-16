#include "err.h"
#include <errno.h>
#include <stdlib.h>

int memoryFailure(void) {
    errno=ENOMEM;
    return -1;
}

int invalidArg(void) {
    errno=EINVAL;
    return -1;
}

int stringNotNullOrEmpty(char const* s) {
    return s!=NULL&&s[0]!='\0';
}

int stringAllowedAs012Seq(char const* s, int n) {
    for(int i=0;i<n;i++) {
        if(s[i]<'0'||s[i]>'2') {
            return 0;
        }
    }
    return 1;
}
