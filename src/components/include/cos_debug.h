#ifndef COS_DEBUG_H

#include <cos_component.h>

#define DEBUG

#define PRINT_FN prints
#include <print.h>
/* Convoluted: We need to pass the __LINE__ through 2 macros to get it
 * to expand to a constant string */
#define STRX(x) #x
#define STR(x) STRX(x)
#define debug_print(str) (PRINT_FN(str __FILE__ ":" STR(__LINE__) ".\n"))
#define BUG() do { debug_print("BUG @ "); *((int *)0) = 0; } while (0);

#ifdef DEBUG
#define assert(node) do { if (!(node)) { debug_print("assert error in @ "); *((int *)0) = 0;} } while(0)
#else 
#define assert(n)
#endif 

#endif
