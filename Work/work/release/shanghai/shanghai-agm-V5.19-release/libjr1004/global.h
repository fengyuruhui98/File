#ifndef _GLOBAL_H_
#define _GLOBAL_H_
 

#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
//typedef unsigned short int UINT2;
typedef unsigned short UINT2;

/* UINT4 defines a four byte word */
//typedef unsigned long int UINT4;
typedef unsigned  int UINT4;

/* BYTE defines a unsigned character */
typedef unsigned char BYTE;

/* internal signed value */
//typedef signed long int signeddigit;
typedef int signeddigit;

#ifndef NULL_PTR
#define NULL_PTR ((POINTER)0)
#endif

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) x = *(&x);
#endif
 
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#endif /* _GLOBAL_H_ */
