
#ifndef UTIL_H
#define UTIL_H

/* INCLUDES */
#include <stdlib.h>
#include <assert.h>

/* DEFINES */
#define S(n) (n==1?"":"s")
#define SS(n,s,m) (n==1?s:m)
#define ASSERT(e) (assert(e))

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define BTOS(b) (b?"true":"false")
// boolean to fail
#define BTOF(b) (b?"   ":"=> ")

#define RETURN_SUCCESS         (0)
#define RETURN_FAILURE_EOF     (1)
#define RETURN_FAILURE_PARSE   (2)
#define RETURN_FAILURE_NEWLINE (3)

/* FUNCTIONS */
void *MALLOC(size_t size);
void *REALLOC(void *ptr,size_t size);
void *FREE(void *ptr);

void EXIT(int code);

#endif
