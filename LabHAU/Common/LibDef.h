#ifndef LIBDEF_H
#define LIBDEF_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <xc.h>
#include <sys/attribs.h>

typedef int8_t                      rslt_t;
typedef struct tm                   tm_t;
typedef const char*                 far_string_t;

typedef struct
{
    int32_t num; // numerator
    int32_t den; // denominator
} fraction_t;

#define public
#define private static

#define __attribute_packed__        __attribute__((packed))
#define __attribute_aligned4__      __attribute__((aligned(4)))
#define __attribute_aligned8__      __attribute__((aligned(8)))
#define __attribute_aligned16__     __attribute__((aligned(16)))

// Result code
#define PROC_DONE                   0
#define PROC_BUSY                   1
#define PROC_REMAIN                 2
#define PROC_CONT                   3
#define PROC_ERR                    (-1)
#define PROC_DRV_ERR                (-2)

#define RESULT_DONE                 0
#define RESULT_BUSY                 1
#define RESULT_REBOOT               2
#define RESULT_ERR                  (-1)

// MIPS memory address convert
#ifndef KVA_TO_PA
#define KVA_TO_PA(v)                ((_paddr_t)(v)&0x1FFFFFFF)
#endif

#ifndef PA_TO_KVA0
#define PA_TO_KVA0(pa)              ((void*)((pa)|0x80000000))
#endif

#define BitTest(x, i)               ((x>>i)&1)
#define BitSet(x, i)                x=x|(1<<i)
#define BitClear(x, i)              x=x&(~(1<<i))

#define membersof(a)                (sizeof(a)/sizeof((a)[0])) // Get the number of elements in an array.
#define indexof(e_p, a)             (e_p-&(a)[0]) // Get the index of given element in an array.

#define MIN(a, b)                   (((a)<(b))?(a):(b))
#define MAX(a, b)                   (((a)>(b))?(a):(b))

#define BIT(pos)                    (1<<(pos))
#define BITFIELD_SET(name, value)   (((value)<<name ## _POS)&name ## _MASK)
#define BITFIELD_GET(name, value)   (((value)&name ## _MASK)>>name ## _POS)

#define typeconvert(type, pt)       (*((type*)pt))

#define NO_INIT                     __attribute__((section(".no_init")))
#define SECTION(a)                  __attribute__((__section__(a)))

#define __make_str(str) #str
#define __make_xstr(str) __make_str(str)

typedef void (*simple_fnc_t) (void);
typedef void (*callback_fnc_t)(uint32_t status, uintptr_t context);

#endif
