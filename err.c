
/**
 * Implementations of functions responsible for handling errors as outlined in the task description.
 */

#include "err.h"
#include <errno.h>
#include <stdlib.h>

/**
 * Function responsible for handling memory failures, enabling modules using it to return -1 with an extra effect of
 * setting errno to ENOMEM in encapsulation.
 */
int memoryFailure(void) {
    errno = ENOMEM;
    return -1;
}

/**
 * Function responsible for handling null failures, enabling modules using it to return NULL with an extra effect of
 * setting errno to 0 in encapsulation.
 */
void *nullFailure(void) {
    errno = 0;
    return NULL;
}

/**
 * Function responsible for handling cases when the calling function receives invalid arguments, enabling it to return
 * -1 with an extra effect of setting errno to EINVAL in encapsulation.
 */
int invalidArg(void) {
    errno = EINVAL;
    return -1;
}

/**
 * Function that checks if a string is not null or empty.
 */
int stringNotNullOrEmpty(char const *s) {
    return s != NULL && s[0] != '\0';
}

/**
 * Function that checks if a string is a valid string over the alphabet {0,1,2}.
 */
int stringAllowedAs012Seq(char const *s, long long unsigned n) {
    for (long long unsigned i = 0; i < n; i++) {
        if (s[i] < '0' || s[i] > '2') {
            return 0;
        }
    }
    return 1;
}
