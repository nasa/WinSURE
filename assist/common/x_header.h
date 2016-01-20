/*
   Definitions which are missing from header files on archaic
   systems such as SUN and ULTRIX

*/
#if !defined(_ZZ_q_x_header_DEFINED)
#define _ZZ_q_x_header_DEFINED

#ifdef _NO_PROTO
extern void usleep();
extern void perror();
#else
extern void usleep(unsigned);
extern void perror(char *);
#endif

#endif
