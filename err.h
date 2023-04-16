#ifndef ERR_H
#define ERR_H

int memoryFailure(void);

int invalidArg(void);

int stringNotNullOrEmpty(char const* s);

int stringAllowedAs012Seq(char const* s, int n);

#endif
