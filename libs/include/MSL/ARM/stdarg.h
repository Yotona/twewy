#ifndef MSL_ARM_STDARG_H
#define MSL_ARM_STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char* va_list;

#define __fourbytealign(n) ((((u32)(n)) + 3U) & ~3U)
#define __va_start(parm)   ((va_list)((char*)((u32)(&parm) & ~3U) + __fourbytealign(sizeof(parm))))

#define va_start(ap, parm) ((ap) = __va_start(parm))
#define va_arg(ap, type)   (*(type*)((ap += __fourbytealign(sizeof(type))) - __fourbytealign(sizeof(type))))
#define va_end(ap)         ((void)0)
#define va_copy(dest, src) dest = src

#ifdef __cplusplus
}
#endif

#endif // MSL_ARM_STDARG_H