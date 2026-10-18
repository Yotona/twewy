#include "Save.h"

typedef struct {
    s8 first;
    s8 second;
} FieldNoiseSequenceEntry;

// clang-format off
const FieldNoiseSequenceEntry NoiseQuota_Ch00_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch01_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch01_Step1[40] = {
    {20, 0}, {10, 0}, {10, 0}, {0, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch02_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch03_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {0, 0}, {0, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch04_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch04_Step1[40] = {
#ifdef REGION_USA
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {0, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
#else
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
#endif
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch05_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch05_Step1[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch05_Step2[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch05_Step3[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch06_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch07_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch07_Step1[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch08_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch08_Step1[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch08_Step2[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch09_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {16, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch10_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch10_Step1[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch11_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch12_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch13_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch14_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch14_Step1[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch15_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch16_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch17_Step0[40] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch18_Step0[40] = {
#ifdef REGION_USA
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 0}, {20, 3}, {16, 2},
#else
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
#endif
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch19_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch20_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 3}, {16, 2}, {20, 0},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch21_Step0[40] = {
    {20, 3}, {10, 2}, {10, 2}, {5, 2}, {16, 2}, {10, 2}, {20, 3}, {16, 2},
    {20, 3}, {16, 2}, {0, 0}, {5, 2}, {10, 2}, {16, 2}, {5, 2}, {10, 2},
    {20, 3}, {30, 4}, {5, 2}, {30, 4}, {0, 0}, {20, 3}, {30, 4}, {0, 0},
    {6, 0}, {6, 0}, {6, 0}, {6, 0}, {6, 0}, {6, 0}, {6, 0}, {6, 1},
    {6, 1}, {6, 1}, {6, 2}, {6, 2}, {6, 3}, {20, 3}, {16, 2}, {20, 3},
};
const FieldNoiseSequenceEntry NoiseQuota_Ch22_Step0[40] = {
    {20, 0}, {10, 0}, {10, 0}, {5, 0}, {16, 0}, {10, 0}, {20, 0}, {16, 0},
    {20, 0}, {16, 0}, {0, 0}, {5, 0}, {10, 0}, {16, 0}, {5, 0}, {10, 0},
    {20, 0}, {30, 0}, {5, 0}, {30, 0}, {0, 0}, {20, 0}, {30, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch03[] = {NoiseQuota_Ch03_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch16[] = {NoiseQuota_Ch16_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch19[] = {NoiseQuota_Ch19_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch15[] = {NoiseQuota_Ch15_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch22[] = {NoiseQuota_Ch22_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch20[] = {NoiseQuota_Ch20_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch21[] = {NoiseQuota_Ch21_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch12[] = {NoiseQuota_Ch12_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch11[] = {NoiseQuota_Ch11_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch09[] = {NoiseQuota_Ch09_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch18[] = {NoiseQuota_Ch18_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch13[] = {NoiseQuota_Ch13_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch17[] = {NoiseQuota_Ch17_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch00[] = {NoiseQuota_Ch00_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch06[] = {NoiseQuota_Ch06_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch02[] = {NoiseQuota_Ch02_Step0, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch04[] = {NoiseQuota_Ch04_Step0, NoiseQuota_Ch04_Step1, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch07[] = {NoiseQuota_Ch07_Step0, NoiseQuota_Ch07_Step1, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch14[] = {NoiseQuota_Ch14_Step0, NoiseQuota_Ch14_Step1, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch10[] = {NoiseQuota_Ch10_Step0, NoiseQuota_Ch10_Step1, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch01[] = {NoiseQuota_Ch01_Step0, NoiseQuota_Ch01_Step1, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch08[] = {NoiseQuota_Ch08_Step0, NoiseQuota_Ch08_Step1, NoiseQuota_Ch08_Step2, NULL};
const FieldNoiseSequenceEntry* NoiseQuotaSteps_Ch05[] = {NoiseQuota_Ch05_Step0, NoiseQuota_Ch05_Step1, NoiseQuota_Ch05_Step2, NoiseQuota_Ch05_Step3, NULL};
const FieldNoiseSequenceEntry* const* NoiseQuotaByChapter[23] = {
    NoiseQuotaSteps_Ch00,
    NoiseQuotaSteps_Ch01,
    NoiseQuotaSteps_Ch02,
    NoiseQuotaSteps_Ch03,
    NoiseQuotaSteps_Ch04,
    NoiseQuotaSteps_Ch05,
    NoiseQuotaSteps_Ch06,
    NoiseQuotaSteps_Ch07,
    NoiseQuotaSteps_Ch08,
    NoiseQuotaSteps_Ch09,
    NoiseQuotaSteps_Ch10,
    NoiseQuotaSteps_Ch11,
    NoiseQuotaSteps_Ch12,
    NoiseQuotaSteps_Ch13,
    NoiseQuotaSteps_Ch14,
    NoiseQuotaSteps_Ch15,
    NoiseQuotaSteps_Ch16,
    NoiseQuotaSteps_Ch17,
    NoiseQuotaSteps_Ch18,
    NoiseQuotaSteps_Ch19,
    NoiseQuotaSteps_Ch20,
    NoiseQuotaSteps_Ch21,
    NoiseQuotaSteps_Ch22,
};
const s32 NoiseQuotaAreaIndex[41] = {
    6, 0, 4, 1, 2, 3, 5, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 24, 37, 38,
    39,
};
// clang-format on

s32 FieldNoise_GetQuotaArea(void) {
    s16 area = gSaveData.fieldArea;

    if ((u32)area >= 41) {
        return 0;
    }
    return NoiseQuotaAreaIndex[area];
}

s32 FieldNoise_GetQuota(s32 kind) {
    u32                                   chapter = gSaveData.chapter;
    const FieldNoiseSequenceEntry* const* chapterTable;

    if (chapter < 23) {
        chapterTable = NoiseQuotaByChapter[chapter];
    } else {
        chapterTable = NoiseQuotaByChapter[chapter - 22];
    }
    const FieldNoiseSequenceEntry* sequence = chapterTable[gSaveData.noiseQuotaStep];
    if (kind == 0) {
        return sequence[FieldNoise_GetQuotaArea()].first;
    }
    if (kind == 1) {
        return sequence[FieldNoise_GetQuotaArea()].second;
    }
    return 0;
}

s16 FieldNoise_GetTotalQuota(void) {
    s16 sum = 0;

    sum += FieldNoise_GetQuota(0);
    sum += FieldNoise_GetQuota(1);
    sum += FieldNoise_GetQuota(2);
    return sum;
}

void FieldNoise_AdvanceQuotaStep(void) {
    u32                                   chapter = gSaveData.chapter;
    const FieldNoiseSequenceEntry* const* sequence;

    if (chapter < 23) {
        sequence = NoiseQuotaByChapter[chapter];
    } else {
        sequence = NoiseQuotaByChapter[chapter - 22];
    }
    s32 next = gSaveData.noiseQuotaStep + 1;
    if (sequence[next] != NULL) {
        gSaveData.noiseQuotaStep = next;
    }
}
