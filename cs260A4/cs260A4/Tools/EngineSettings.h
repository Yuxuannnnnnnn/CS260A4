//define debug mode
#define DEBUG

//if debug mode is defined, ASSERT will print something, otherwise will not
#ifdef DEBUG
#define ASSERT(arg)    printf(arg)
#else
#define ASSERT(arg)
#endif
