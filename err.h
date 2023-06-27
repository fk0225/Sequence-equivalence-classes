#ifndef ERR_H
#define ERR_H

/**
 * @return exit code specified in the task description (in all cases equal to -1).
 */
int memoryFailure(void);

/**
 * @return void pointer acting as exit code specified in the task description (in all cases equal to NULL).
 */
void* nullFailure(void);

/**
 * @return exit code specified in the task description (in all cases equal to -1).
 */
int invalidArg(void);

/**
 * @param s: the string being tested.
 * @return 1 if the string is not (null or empty), 0 otherwise.
 */
int stringNotNullOrEmpty(char const* s);

/**
 * @param s: the string being tested.
 * @return 1 if the string is a valid sequence over the alphabet {0,1,2}, 0 otherwise.
 */
int stringAllowedAs012Seq(char const* s, long long unsigned n);

#endif
