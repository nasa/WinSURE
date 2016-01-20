/*
   Definitions which are missing from "stdio.h" on archaic systems such
   as SUN and ULTRIX

*/
#if !defined(_ZZ_q_x_stdio_DEFINED)
#define _ZZ_q_x_stdio_DEFINED

/* extern int fprintf(); */
/* extern int printf(); */
/* #define getch() getchar() */

#ifdef _NO_PROTO
/* extern int system(); */
/* extern int fputc(); */
/* extern long strtol(); */
/* extern int toupper(); */
/* extern int memcmp(); */
/* extern void *memcpy(); */
/* extern void *memmove(); */
/* extern char *memset(); */
/* extern int fclose(); */
/* extern int setvbuf(); */
/* extern int remove(); */
/* extern void _flsbuf(); */
/* extern int _filbuf(); */
/* extern int fgetc(); */
/* extern int fseek(); */
/* extern void rewind(); */
/* extern size_t fread(); */
/* extern size_t fwrite(); */
extern void cfree(); 
#else
/* extern void fflush(FILE *); */
/* extern int system(char *); */
/* extern int fputc(int,FILE *); */
/* extern long strtol(char *,char **,int); */
/* extern int toupper(int); */
/* extern int memcmp(const void *,const void *,size_t); */
/* extern void *memcpy(void *,const void *,size_t); */
/* extern void *memmove(void *,const void *,size_t); */
/* extern char *memset(void *,int,size_t); */
/* extern int fclose(FILE *); */
/* extern int setvbuf(FILE *,char *,int,size_t); */
/* extern int remove(char *); */
/* extern void _flsbuf(unsigned char,FILE *); */
/* extern int _filbuf(FILE *); */
/* extern int fgetc(FILE *); */
/* extern int fseek(FILE *,long,int); */
/* extern void rewind(FILE *); */
/* extern size_t fread(void *,size_t,size_t,FILE *); */
/* extern size_t fwrite(void *,size_t,size_t,FILE *); */
extern void cfree(void *);
#endif

#endif


