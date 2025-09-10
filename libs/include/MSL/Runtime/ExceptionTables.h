#ifndef MSL_RUNTIME_EXCEPTIONTABLES_H
#define MSL_RUNTIME_EXCEPTIONTABLES_H

typedef struct ExceptionTableIndex {
    char*         function_address;
    unsigned long function_size;
    char*         exception_table;
} ExceptionTableIndex;

#define FUNCTION_SIZE_MASK 0xFFFFFFFE
#define DIRECT_STORE_MASK  0x00000001

#define FUNCTION_SIZE(eti) ((eti)->function_size & FUNCTION_SIZE_MASK)
#define DIRECT_STORED(eti) (((eti)->function_size & DIRECT_STORE_MASK) != 0)

typedef unsigned char exaction_type;
#define EXACTION_ENDBIT  0x80
#define EXACTION_OPT1BIT 0x40
#define EXACTION_OPT2BIT 0x20
#define EXACTION_MASK    0x1F

#define REGCOND EXACTION_OPT1BIT
#define REGPTR  EXACTION_OPT2BIT

#define EXACTION_ENDOFLIST             0
#define EXACTION_BRANCH                1
#define EXACTION_DESTROYLOCAL          2
#define EXACTION_DESTROYLOCALCOND      3
#define EXACTION_DESTROYLOCALPOINTER   4
#define EXACTION_DESTROYLOCALARRAY     5
#define EXACTION_DESTROYBASE           6
#define EXACTION_DESTROYMEMBER         7
#define EXACTION_DESTROYMEMBERCOND     8
#define EXACTION_DESTROYMEMBERARRAY    9
#define EXACTION_DELETEPOINTER         10
#define EXACTION_DELETEPOINTERCOND     11
#define EXACTION_CATCHBLOCK            12
#define EXACTION_ACTIVECATCHBLOCK      13
#define EXACTION_TERMINATE             14
#define EXACTION_SPECIFICATION         15
#define EXACTION_DESTROYBASEVTTOBJECT  16
#define EXACTION_DESTROYBASEVTTPOINTER 17
#define EXACTION_DESTROYVLA            18
#define EXACTION_FREEVLA               19

#endif // MSL_RUNTIME_EXCEPTIONTABLES_H