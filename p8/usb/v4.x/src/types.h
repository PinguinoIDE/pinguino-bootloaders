#ifndef TYPES_H_
#define TYPES_H_

#define WORDSIZE        2    // 1 word = 2 bytes

//typedef unsigned char byte;
//typedef unsigned int  word;
//typedef unsigned long dword;

typedef unsigned char   u8;  //byte;
typedef unsigned int    u16; //word;
typedef unsigned long   u32; //dword;

typedef char            s8;  //byte;
typedef int             s16; //word;
typedef long            s32; //dword;

//#define TRUE            1
//#define FALSE           !TRUE

#define FALSE           0
#define TRUE            !FALSE

#endif
