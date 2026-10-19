#ifndef FIELD_SCENE_FIELDAREA_H
#define FIELD_SCENE_FIELDAREA_H

#include "Field/FieldTypes.h"

void       func_ov030_020b7aac(s32* arg0);
void       func_ov030_020b7ac8(void);
s16        func_ov030_020b7b00(void);
s16        func_ov030_020b7b20(void);
s16        func_ov030_020b7b40(void);
s16        func_ov030_020b7b60(void);
s32        func_ov030_020b7b80(void);
s32        func_ov030_020b7ba8(void);
s16        func_ov030_020b7bc4(void);
s16        func_ov030_020b7bf8(void);
s32        func_ov030_020b7c18(void);
u16        func_ov030_020b7c34(s32 area);
void       func_ov030_020b7c58(s32 unused, s32* out1, s32* out2, s32* out3);
s32        func_ov030_020b7c9c(s32 value, s32 unusedY);
s32        func_ov030_020b7d04(s32 value);
s32        func_ov030_020b7dd8(s32 arg0);
FieldExit* func_ov030_020b8358(s32 arg0, FieldPos pos, s32 arg3, s32* arg4);
FieldExit* func_ov030_020b84a4(s32 arg0, s32* arg1, s32* arg2);
FieldExit* func_ov030_020b85a8(s32 arg0, s32* arg1);
s32        func_ov030_020b86bc(FieldPos* arg0, s16* arg1, s32* arg2, s32 arg3);
void       func_ov030_020b88e8(FieldExit* exit);

#endif // FIELD_SCENE_FIELDAREA_H
