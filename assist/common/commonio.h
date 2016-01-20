/* $Log$
 */
#if !defined(_ZZ_q_commonio_DEFINED)
#define _ZZ_q_commonio_DEFINED

#if ! ( defined(WITH_TAE_DCLS) || defined(WITH_X_DCLS) )
#define FALSE (Boolean) 0
#define TRUE  (Boolean) 1
typedef unsigned char Boolean;
#endif

#define f_FALSE (flagtyp) 0
#define f_TRUE  (flagtyp) 1
typedef unsigned char flagtyp;

#ifdef _NO_PROTO
extern short siglen();
extern flagtyp safe_strcat_truncated();
#else
extern short siglen(char *,short);
extern flagtyp safe_strcat_truncated(char *,char *,int);
#endif

#define BufEnd(buf) ((buf)+strlen(buf))

#define bitset(item,mask) (flagtyp) (((item) & (mask)) != 0x0)

typedef char *charptr;

#endif
