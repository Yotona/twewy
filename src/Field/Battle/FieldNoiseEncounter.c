#include "Field/Battle/FieldNoiseEncounter.h"
#include "Engine/Math/Random.h"
#include "Field/Tasks/Fld_Piranha.h"
#include "Save.h"

typedef struct {
    s16 encounter;
    s16 weight;
} FieldNoiseEntry;

typedef struct {
    /* 0x00 */ const FieldNoiseEntry* const* tables[41];
} FieldNoiseTableSet; // Size: 0xA4

// clang-format off
const FieldNoiseEntry NoiseEncounters_Area39_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch17[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch16[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch05[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch10[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch03[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch11[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch09[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch06[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch18[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch12[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch20[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch19[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch04[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch01[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch15[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch14[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch02[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch08[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch00[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch07[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch13[] = {{-1, 0}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch13[] = {{376, 35}, {377, 35}, {378, 30}, {-1, 3}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch13[] = {{402, 30}, {403, 40}, {404, 30}, {-1, 3}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch21[] = {{1227, 30}, {1228, 30}, {1229, 20}, {1230, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch10[] = {{332, 30}, {333, 30}, {334, 20}, {335, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch04[] = {{255, 30}, {256, 30}, {257, 20}, {258, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch05[] = {{257, 20}, {258, 20}, {259, 30}, {260, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch08[] = {{420, 25}, {421, 20}, {422, 35}, {423, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch09[] = {{422, 20}, {423, 35}, {424, 25}, {425, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch11[] = {{426, 30}, {427, 15}, {428, 20}, {429, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch21[] = {{807, 25}, {808, 25}, {813, 40}, {814, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch05[] = {{225, 20}, {226, 20}, {227, 30}, {228, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch09[] = {{430, 30}, {431, 30}, {432, 20}, {433, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch05[] = {{249, 20}, {250, 20}, {251, 30}, {252, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch10[] = {{432, 35}, {433, 15}, {434, 25}, {435, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch11[] = {{434, 20}, {435, 20}, {436, 30}, {437, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area26_Ch21[] = {{1231, 15}, {1232, 35}, {1233, 15}, {1234, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch21[] = {{830, 30}, {831, 30}, {836, 20}, {837, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch04[] = {{263, 30}, {264, 30}, {265, 20}, {266, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch05[] = {{265, 30}, {266, 30}, {267, 20}, {268, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch06[] = {{267, 30}, {268, 30}, {269, 20}, {270, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch08[] = {{443, 15}, {444, 15}, {445, 35}, {446, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch09[] = {{445, 30}, {446, 25}, {447, 25}, {448, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch10[] = {{447, 20}, {448, 20}, {449, 35}, {450, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch11[] = {{449, 15}, {450, 35}, {451, 25}, {452, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch05[] = {{210, 15}, {211, 45}, {212, 20}, {213, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch07[] = {{326, 25}, {327, 25}, {328, 25}, {329, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch11[] = {{548, 20}, {549, 20}, {550, 30}, {551, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch21[] = {{861, 35}, {862, 15}, {867, 25}, {868, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area36_Ch21[] = {{1271, 15}, {1272, 35}, {1273, 15}, {1274, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch09[] = {{490, 25}, {491, 25}, {492, 20}, {493, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch11[] = {{453, 20}, {454, 40}, {455, 20}, {456, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area33_Ch21[] = {{1259, 30}, {1260, 30}, {1261, 30}, {1262, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch08[] = {{390, 30}, {391, 30}, {392, 20}, {393, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area27_Ch21[] = {{1235, 20}, {1236, 35}, {1237, 10}, {1238, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch21[] = {{884, 25}, {885, 25}, {890, 5}, {891, 45}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch03[] = {{271, 30}, {272, 30}, {273, 30}, {274, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch04[] = {{239, 30}, {240, 30}, {241, 20}, {242, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch05[] = {{274, 20}, {275, 30}, {276, 25}, {277, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch06[] = {{276, 20}, {277, 20}, {278, 30}, {279, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch08[] = {{462, 25}, {463, 25}, {464, 20}, {465, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch09[] = {{464, 25}, {465, 15}, {466, 25}, {467, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch09[] = {{544, 35}, {545, 25}, {546, 25}, {547, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch10[] = {{466, 35}, {467, 15}, {468, 25}, {469, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch11[] = {{468, 20}, {469, 20}, {470, 30}, {471, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch21[] = {{911, 25}, {912, 25}, {917, 10}, {918, 40}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch03[] = {{280, 30}, {281, 30}, {282, 30}, {283, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch05[] = {{283, 15}, {284, 45}, {285, 20}, {286, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch06[] = {{285, 20}, {286, 20}, {287, 30}, {288, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch08[] = {{472, 30}, {473, 30}, {474, 20}, {475, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch09[] = {{474, 40}, {475, 20}, {476, 20}, {477, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch10[] = {{476, 25}, {477, 25}, {478, 25}, {479, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch11[] = {{478, 20}, {479, 20}, {480, 30}, {481, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area28_Ch21[] = {{1239, 20}, {1240, 30}, {1241, 20}, {1242, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch14[] = {{617, 20}, {618, 20}, {619, 15}, {620, 45}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch21[] = {{938, 60}, {939, 20}, {944, 10}, {945, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch03[] = {{289, 30}, {290, 30}, {291, 30}, {292, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch05[] = {{292, 15}, {293, 45}, {294, 20}, {295, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch06[] = {{294, 20}, {295, 20}, {296, 30}, {297, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch09[] = {{482, 30}, {483, 30}, {484, 20}, {485, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch10[] = {{484, 30}, {485, 30}, {486, 20}, {487, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch11[] = {{486, 20}, {487, 20}, {488, 25}, {489, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch21[] = {{965, 20}, {966, 20}, {971, 30}, {972, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch01[] = {{298, 30}, {299, 30}, {300, 25}, {301, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch06[] = {{302, 30}, {303, 25}, {304, 25}, {305, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch09[] = {{514, 25}, {515, 25}, {516, 25}, {517, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch10[] = {{516, 25}, {517, 25}, {518, 25}, {519, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch11[] = {{518, 25}, {519, 25}, {520, 25}, {521, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area34_Ch21[] = {{1263, 20}, {1264, 20}, {1265, 50}, {1266, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch14[] = {{1032, 20}, {1033, 30}, {1034, 30}, {1035, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area29_Ch21[] = {{1243, 30}, {1244, 30}, {1245, 30}, {1246, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch21[] = {{1061, 25}, {1062, 25}, {1067, 25}, {1068, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch01[] = {{306, 20}, {307, 25}, {308, 30}, {309, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch06[] = {{310, 25}, {311, 25}, {312, 25}, {313, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch09[] = {{522, 25}, {523, 25}, {524, 25}, {525, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch10[] = {{524, 25}, {525, 25}, {526, 25}, {527, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch11[] = {{526, 25}, {527, 25}, {528, 25}, {529, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch14[] = {{1069, 30}, {1070, 20}, {1071, 30}, {1072, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch21[] = {{1098, 25}, {1099, 25}, {1104, 5}, {1105, 45}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch02[] = {{314, 10}, {315, 35}, {316, 35}, {317, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch09[] = {{530, 35}, {531, 25}, {532, 20}, {533, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch10[] = {{532, 30}, {533, 30}, {534, 20}, {535, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch11[] = {{534, 15}, {535, 15}, {536, 35}, {537, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch03[] = {{207, 30}, {208, 30}, {209, 30}, {210, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch21[] = {{1211, 30}, {1220, 60}, {1221, 7}, {1222, 3}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch11[] = {{562, 35}, {563, 25}, {564, 25}, {565, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch08[] = {{328, 20}, {329, 40}, {330, 20}, {331, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area30_Ch21[] = {{1247, 60}, {1248, 15}, {1249, 15}, {1250, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch21[] = {{609, 40}, {610, 30}, {615, 20}, {616, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch03[] = {{216, 30}, {217, 30}, {218, 30}, {219, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch02[] = {{318, 15}, {319, 25}, {320, 45}, {321, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch05[] = {{219, 15}, {220, 45}, {221, 20}, {222, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch07[] = {{347, 30}, {348, 30}, {349, 30}, {350, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch09[] = {{350, 20}, {351, 30}, {352, 20}, {353, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch10[] = {{352, 20}, {353, 30}, {354, 25}, {355, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch11[] = {{354, 20}, {355, 20}, {356, 40}, {357, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch10[] = {{546, 20}, {547, 20}, {548, 25}, {549, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch21[] = {{646, 30}, {647, 20}, {652, 20}, {653, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch06[] = {{227, 30}, {228, 30}, {229, 20}, {230, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch10[] = {{368, 20}, {369, 20}, {370, 30}, {371, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch11[] = {{370, 25}, {371, 25}, {372, 20}, {373, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch12[] = {{372, 15}, {373, 20}, {374, 35}, {375, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch06[] = {{221, 20}, {222, 20}, {223, 30}, {224, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch21[] = {{1168, 30}, {1169, 40}, {1174, 15}, {1175, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch21[] = {{1211, 30}, {1220, 60}, {1221, 7}, {1222, 3}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch21[] = {{677, 30}, {678, 30}, {683, 30}, {684, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch21[] = {{1211, 30}, {1220, 60}, {1221, 7}, {1222, 3}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch01[] = {{231, 25}, {232, 25}, {233, 25}, {234, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch02[] = {{322, 20}, {323, 25}, {324, 25}, {325, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch06[] = {{235, 30}, {236, 30}, {237, 20}, {238, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch07[] = {{379, 20}, {380, 15}, {381, 50}, {382, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch09[] = {{382, 10}, {383, 25}, {384, 25}, {385, 40}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch10[] = {{384, 20}, {385, 25}, {386, 30}, {387, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch11[] = {{386, 20}, {387, 20}, {388, 30}, {389, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch14[] = {{685, 20}, {686, 10}, {687, 40}, {688, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch09[] = {{558, 40}, {559, 20}, {560, 20}, {561, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch10[] = {{560, 20}, {561, 20}, {562, 40}, {563, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch21[] = {{714, 20}, {715, 20}, {720, 30}, {721, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area35_Ch21[] = {{1267, 15}, {1268, 20}, {1269, 15}, {1270, 50}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch10[] = {{492, 20}, {493, 30}, {494, 25}, {495, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch11[] = {{494, 20}, {495, 20}, {496, 30}, {497, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area31_Ch21[] = {{1251, 30}, {1252, 30}, {1253, 10}, {1254, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch14[] = {{580, 30}, {581, 20}, {582, 20}, {583, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch17[] = {{988, 25}, {989, 25}, {990, 25}, {991, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch21[] = {{1203, 30}, {1204, 30}, {1209, 30}, {1210, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch21[] = {{1133, 20}, {1134, 20}, {1139, 30}, {1140, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch21[] = {{982, 20}, {983, 30}, {992, 20}, {993, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch09[] = {{498, 25}, {499, 25}, {500, 25}, {501, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch10[] = {{500, 25}, {501, 25}, {502, 25}, {503, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch11[] = {{502, 25}, {503, 25}, {504, 25}, {505, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch17[] = {{1009, 25}, {1010, 25}, {1011, 25}, {1012, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch21[] = {{1003, 40}, {1004, 15}, {1013, 15}, {1014, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch13[] = {{572, 25}, {573, 25}, {574, 25}, {575, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch09[] = {{506, 25}, {507, 25}, {508, 35}, {509, 15}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch10[] = {{508, 30}, {509, 30}, {510, 20}, {511, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch11[] = {{510, 25}, {511, 25}, {512, 25}, {513, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area24_Ch21[] = {{1223, 30}, {1224, 30}, {1225, 20}, {1226, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch21[] = {{1024, 30}, {1025, 30}, {1030, 10}, {1031, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch11[] = {{334, 25}, {335, 25}, {336, 25}, {337, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch05[] = {{241, 20}, {242, 20}, {243, 30}, {244, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch06[] = {{243, 20}, {244, 20}, {245, 30}, {246, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch09[] = {{392, 20}, {393, 25}, {394, 30}, {395, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch10[] = {{394, 20}, {395, 20}, {396, 30}, {397, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch11[] = {{396, 20}, {397, 20}, {398, 25}, {399, 35}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch12[] = {{398, 30}, {399, 20}, {400, 20}, {401, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch09[] = {{366, 25}, {367, 30}, {368, 25}, {369, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch21[] = {{745, 20}, {746, 40}, {751, 30}, {752, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area25_Ch13[] = {{576, 25}, {577, 25}, {578, 25}, {579, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch04[] = {{247, 30}, {248, 30}, {249, 20}, {250, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch09[] = {{330, 25}, {331, 25}, {332, 20}, {333, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch06[] = {{251, 20}, {252, 20}, {253, 30}, {254, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch08[] = {{405, 30}, {406, 20}, {407, 30}, {408, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch09[] = {{407, 20}, {408, 20}, {409, 30}, {410, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch10[] = {{409, 20}, {410, 20}, {411, 30}, {412, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch11[] = {{411, 20}, {412, 20}, {413, 30}, {414, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area32_Ch21[] = {{1255, 20}, {1256, 10}, {1257, 60}, {1258, 10}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch08[] = {{364, 25}, {365, 25}, {366, 30}, {367, 20}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch21[] = {{776, 25}, {777, 25}, {782, 25}, {783, 25}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch06[] = {{259, 20}, {260, 20}, {261, 30}, {262, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch10[] = {{424, 20}, {425, 20}, {426, 30}, {427, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch06[] = {{212, 20}, {213, 20}, {214, 30}, {215, 30}, {-1, 4}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch15[] = {{1141, 20}, {1142, 20}, {1143, 40}, {1144, 20}, {1145, 50}, {1146, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch16[] = {{1143, 20}, {1144, 20}, {1147, 20}, {1148, 40}, {1149, 50}, {1150, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch17[] = {{1147, 30}, {1148, 20}, {1151, 20}, {1152, 30}, {1153, 50}, {1154, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch16[] = {{654, 20}, {655, 20}, {656, 40}, {657, 20}, {658, 50}, {659, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch17[] = {{656, 20}, {657, 20}, {660, 30}, {661, 30}, {662, 50}, {663, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch15[] = {{687, 20}, {688, 30}, {689, 30}, {690, 20}, {691, 50}, {692, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch16[] = {{689, 20}, {690, 20}, {693, 40}, {694, 20}, {695, 50}, {696, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch17[] = {{693, 20}, {694, 40}, {697, 20}, {698, 20}, {699, 50}, {700, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch15[] = {{1176, 10}, {1177, 40}, {1178, 40}, {1179, 10}, {1180, 50}, {1181, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch16[] = {{1178, 30}, {1179, 20}, {1182, 20}, {1183, 30}, {1184, 50}, {1185, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch17[] = {{1182, 40}, {1183, 40}, {1186, 10}, {1187, 10}, {1188, 50}, {1189, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch15[] = {{582, 20}, {583, 20}, {584, 30}, {585, 30}, {586, 50}, {587, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch16[] = {{722, 30}, {723, 30}, {724, 20}, {725, 20}, {726, 50}, {727, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch17[] = {{724, 20}, {725, 20}, {728, 40}, {729, 20}, {730, 50}, {731, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch16[] = {{753, 20}, {754, 20}, {755, 45}, {756, 15}, {757, 50}, {758, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch17[] = {{755, 20}, {756, 30}, {759, 30}, {760, 20}, {761, 50}, {762, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch16[] = {{784, 20}, {785, 30}, {786, 20}, {787, 30}, {788, 50}, {789, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch17[] = {{786, 15}, {787, 15}, {790, 25}, {791, 45}, {792, 50}, {793, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch16[] = {{838, 20}, {839, 20}, {840, 40}, {841, 20}, {842, 50}, {843, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch17[] = {{840, 20}, {841, 40}, {844, 20}, {845, 20}, {846, 50}, {847, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch17[] = {{892, 20}, {893, 40}, {894, 20}, {895, 20}, {896, 50}, {897, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch17[] = {{919, 20}, {920, 30}, {921, 20}, {922, 30}, {923, 50}, {924, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch17[] = {{946, 20}, {947, 20}, {948, 30}, {949, 30}, {950, 50}, {951, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch15[] = {{1034, 40}, {1035, 10}, {1036, 10}, {1037, 40}, {1038, 50}, {1039, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch16[] = {{1036, 20}, {1037, 20}, {1040, 30}, {1041, 30}, {1042, 50}, {1043, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch17[] = {{1040, 20}, {1041, 20}, {1044, 30}, {1045, 30}, {1046, 50}, {1047, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch15[] = {{1071, 20}, {1072, 30}, {1073, 20}, {1074, 30}, {1075, 50}, {1076, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch16[] = {{1073, 30}, {1074, 20}, {1077, 30}, {1078, 20}, {1079, 50}, {1080, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch17[] = {{1077, 20}, {1078, 30}, {1081, 20}, {1082, 30}, {1083, 50}, {1084, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch15[] = {{1106, 20}, {1107, 20}, {1108, 30}, {1109, 30}, {1110, 50}, {1111, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch16[] = {{1108, 20}, {1109, 20}, {1112, 30}, {1113, 30}, {1114, 50}, {1115, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch17[] = {{1112, 20}, {1113, 20}, {1116, 20}, {1117, 40}, {1118, 50}, {1119, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch16[] = {{584, 20}, {585, 20}, {588, 30}, {589, 30}, {590, 50}, {591, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch17[] = {{588, 20}, {589, 20}, {592, 30}, {593, 30}, {594, 50}, {595, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch15[] = {{619, 10}, {620, 40}, {621, 10}, {622, 40}, {623, 50}, {624, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch16[] = {{621, 20}, {622, 20}, {625, 30}, {626, 30}, {627, 50}, {628, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch17[] = {{625, 20}, {626, 20}, {629, 40}, {630, 20}, {631, 50}, {632, 50}, {-1, 6}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch13[] = {{537, 20}, {538, 35}, {539, 25}, {540, 20}, {541, 40}, {542, 30}, {543, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch12[] = {{336, 25}, {337, 40}, {338, 25}, {339, 10}, {340, 35}, {341, 35}, {342, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch13[] = {{357, 15}, {358, 25}, {359, 35}, {360, 25}, {361, 35}, {362, 35}, {363, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch13[] = {{551, 15}, {552, 25}, {553, 30}, {554, 30}, {555, 25}, {556, 35}, {557, 40}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch13[] = {{565, 25}, {566, 25}, {567, 25}, {568, 25}, {569, 35}, {570, 35}, {571, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch12[] = {{413, 15}, {414, 15}, {415, 35}, {416, 35}, {417, 35}, {418, 35}, {419, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch13[] = {{338, 20}, {339, 20}, {342, 30}, {343, 30}, {344, 30}, {345, 50}, {346, 20}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch12[] = {{436, 20}, {437, 20}, {438, 25}, {439, 35}, {440, 35}, {441, 30}, {442, 35}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch12[] = {{455, 20}, {456, 20}, {457, 30}, {458, 30}, {459, 35}, {460, 35}, {461, 30}, {-1, 7}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch18[] = {{973, 25}, {974, 25}, {977, 50}, {978, 50}, {980, 50}, {981, 50}, {990, 25}, {991, 25}, {-1, 8}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch18[] = {{994, 25}, {995, 25}, {998, 50}, {999, 50}, {1001, 50}, {1002, 50}, {1011, 25}, {1012, 25}, {-1, 8}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch19[] = {{1120, 10}, {1121, 30}, {1124, 50}, {1127, 30}, {1128, 30}, {1129, 30}, {1130, 20}, {1131, 50}, {1132, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch20[] = {{1127, 40}, {1128, 20}, {1130, 10}, {1133, 20}, {1134, 20}, {1135, 30}, {1136, 60}, {1137, 50}, {1138, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch19[] = {{596, 20}, {597, 30}, {600, 30}, {603, 30}, {604, 20}, {605, 20}, {606, 50}, {607, 50}, {608, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch20[] = {{603, 20}, {604, 20}, {606, 20}, {609, 45}, {610, 15}, {611, 40}, {612, 40}, {613, 50}, {614, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch19[] = {{633, 20}, {634, 15}, {637, 40}, {640, 20}, {641, 45}, {642, 20}, {643, 40}, {644, 50}, {645, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch20[] = {{640, 40}, {641, 20}, {643, 40}, {646, 20}, {647, 20}, {648, 40}, {649, 20}, {650, 50}, {651, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch18[] = {{1151, 40}, {1152, 15}, {1155, 15}, {1156, 30}, {1157, 30}, {1158, 40}, {1159, 30}, {1160, 50}, {1161, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch19[] = {{1155, 20}, {1156, 40}, {1159, 30}, {1162, 20}, {1163, 20}, {1164, 30}, {1165, 40}, {1166, 50}, {1167, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch18[] = {{660, 30}, {661, 30}, {664, 10}, {665, 30}, {666, 30}, {667, 40}, {668, 30}, {669, 50}, {670, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch19[] = {{664, 15}, {665, 25}, {668, 40}, {671, 30}, {672, 30}, {673, 20}, {674, 40}, {675, 50}, {676, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch18[] = {{697, 20}, {698, 20}, {701, 20}, {702, 40}, {703, 40}, {704, 30}, {705, 30}, {706, 50}, {707, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch19[] = {{701, 20}, {702, 25}, {705, 40}, {708, 30}, {709, 25}, {710, 20}, {711, 40}, {712, 50}, {713, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch18[] = {{1186, 15}, {1187, 15}, {1190, 50}, {1191, 20}, {1192, 40}, {1193, 30}, {1194, 30}, {1195, 50}, {1196, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch19[] = {{1190, 30}, {1191, 20}, {1194, 50}, {1197, 30}, {1198, 20}, {1199, 30}, {1200, 20}, {1201, 50}, {1202, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch19[] = {{973, 30}, {974, 30}, {975, 20}, {976, 20}, {977, 30}, {978, 45}, {979, 25}, {980, 50}, {981, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area14_Ch20[] = {{975, 30}, {976, 30}, {979, 30}, {982, 20}, {983, 20}, {984, 30}, {985, 40}, {986, 50}, {987, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch20[] = {{996, 15}, {997, 35}, {1000, 25}, {1003, 35}, {1004, 15}, {1005, 25}, {1006, 50}, {1007, 50}, {1008, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch19[] = {{1015, 40}, {1016, 20}, {1017, 20}, {1018, 20}, {1019, 35}, {1020, 45}, {1021, 20}, {1022, 50}, {1023, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch18[] = {{728, 30}, {729, 20}, {732, 15}, {733, 35}, {734, 30}, {735, 30}, {736, 40}, {737, 50}, {738, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch19[] = {{732, 20}, {733, 20}, {736, 40}, {739, 20}, {740, 40}, {741, 20}, {742, 40}, {743, 50}, {744, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch18[] = {{759, 20}, {760, 30}, {763, 20}, {764, 30}, {765, 40}, {766, 30}, {767, 30}, {768, 50}, {769, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch19[] = {{763, 30}, {764, 30}, {767, 40}, {770, 20}, {771, 20}, {772, 30}, {773, 30}, {774, 50}, {775, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch18[] = {{790, 20}, {791, 40}, {794, 20}, {795, 20}, {796, 30}, {797, 30}, {798, 40}, {799, 50}, {800, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch19[] = {{794, 20}, {795, 20}, {798, 20}, {801, 40}, {802, 20}, {803, 35}, {804, 45}, {805, 50}, {806, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch18[] = {{815, 20}, {816, 20}, {817, 20}, {818, 40}, {819, 35}, {820, 35}, {821, 30}, {822, 50}, {823, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch19[] = {{817, 30}, {818, 20}, {821, 40}, {824, 20}, {825, 30}, {826, 20}, {827, 40}, {828, 50}, {829, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area07_Ch20[] = {{824, 35}, {825, 35}, {827, 30}, {830, 15}, {831, 15}, {832, 20}, {833, 50}, {834, 50}, {835, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch18[] = {{844, 20}, {845, 20}, {848, 20}, {849, 40}, {850, 40}, {851, 30}, {852, 30}, {853, 50}, {854, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch19[] = {{848, 20}, {849, 30}, {852, 40}, {855, 20}, {856, 30}, {857, 20}, {858, 40}, {859, 50}, {860, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area08_Ch20[] = {{855, 30}, {856, 25}, {858, 30}, {861, 25}, {862, 20}, {863, 35}, {864, 35}, {865, 50}, {866, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch18[] = {{869, 20}, {870, 20}, {871, 30}, {872, 30}, {873, 30}, {874, 40}, {875, 30}, {876, 50}, {877, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch19[] = {{871, 20}, {872, 20}, {875, 40}, {878, 20}, {879, 40}, {880, 30}, {881, 30}, {882, 50}, {883, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area09_Ch20[] = {{878, 20}, {879, 30}, {881, 30}, {884, 30}, {885, 20}, {886, 30}, {887, 40}, {888, 50}, {889, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area40_Ch20[] = {{1211, 20}, {1212, 30}, {1213, 30}, {1214, 20}, {1215, 40}, {1216, 30}, {1217, 30}, {1218, 50}, {1219, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch18[] = {{894, 20}, {895, 20}, {898, 30}, {899, 30}, {900, 30}, {901, 40}, {902, 30}, {903, 50}, {904, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch19[] = {{898, 40}, {899, 20}, {902, 40}, {905, 20}, {906, 20}, {907, 20}, {908, 40}, {909, 50}, {910, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area11_Ch20[] = {{905, 20}, {906, 20}, {908, 30}, {911, 30}, {912, 30}, {913, 25}, {914, 45}, {915, 50}, {916, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch18[] = {{921, 20}, {922, 20}, {925, 30}, {926, 30}, {927, 30}, {928, 30}, {929, 40}, {930, 50}, {931, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch19[] = {{925, 20}, {926, 20}, {929, 50}, {932, 30}, {933, 30}, {934, 30}, {935, 20}, {936, 50}, {937, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area12_Ch20[] = {{932, 30}, {933, 30}, {935, 20}, {938, 30}, {939, 10}, {940, 60}, {941, 20}, {942, 50}, {943, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch18[] = {{948, 20}, {949, 20}, {952, 30}, {953, 30}, {954, 30}, {955, 40}, {956, 30}, {957, 50}, {958, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch19[] = {{952, 20}, {953, 20}, {956, 50}, {959, 40}, {960, 20}, {961, 30}, {962, 20}, {963, 50}, {964, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area13_Ch20[] = {{959, 20}, {960, 20}, {962, 20}, {965, 30}, {966, 30}, {967, 40}, {968, 40}, {969, 50}, {970, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch18[] = {{1044, 30}, {1045, 30}, {1048, 30}, {1049, 10}, {1050, 35}, {1051, 35}, {1052, 30}, {1053, 50}, {1054, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch19[] = {{1048, 30}, {1049, 10}, {1052, 40}, {1055, 30}, {1056, 30}, {1057, 20}, {1058, 40}, {1059, 50}, {1060, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area17_Ch20[] = {{1055, 20}, {1056, 20}, {1058, 20}, {1061, 30}, {1062, 30}, {1063, 40}, {1064, 40}, {1065, 50}, {1066, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch18[] = {{1081, 20}, {1082, 40}, {1085, 20}, {1086, 20}, {1087, 40}, {1088, 30}, {1089, 30}, {1090, 50}, {1091, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch19[] = {{1085, 30}, {1086, 30}, {1089, 60}, {1092, 20}, {1093, 20}, {1094, 20}, {1095, 20}, {1096, 50}, {1097, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area18_Ch20[] = {{1092, 20}, {1093, 15}, {1095, 20}, {1098, 15}, {1099, 50}, {1100, 20}, {1101, 60}, {1102, 50}, {1103, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area39_Ch20[] = {{1211, 20}, {1212, 30}, {1213, 30}, {1214, 20}, {1215, 40}, {1216, 30}, {1217, 30}, {1218, 50}, {1219, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area19_Ch18[] = {{1116, 20}, {1117, 30}, {1120, 20}, {1121, 30}, {1122, 40}, {1123, 30}, {1124, 30}, {1125, 50}, {1126, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area01_Ch18[] = {{592, 30}, {593, 10}, {596, 20}, {597, 40}, {598, 30}, {599, 30}, {600, 40}, {601, 50}, {602, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area03_Ch18[] = {{629, 30}, {630, 15}, {633, 40}, {634, 15}, {635, 40}, {636, 30}, {637, 30}, {638, 50}, {639, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area38_Ch20[] = {{1211, 20}, {1212, 30}, {1213, 30}, {1214, 20}, {1215, 40}, {1216, 30}, {1217, 30}, {1218, 50}, {1219, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area21_Ch20[] = {{1162, 30}, {1163, 30}, {1165, 30}, {1168, 10}, {1169, 30}, {1170, 40}, {1171, 30}, {1172, 50}, {1173, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area04_Ch20[] = {{671, 20}, {672, 30}, {674, 20}, {677, 25}, {678, 25}, {679, 30}, {680, 50}, {681, 50}, {682, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area05_Ch20[] = {{708, 20}, {709, 20}, {711, 20}, {714, 30}, {715, 30}, {716, 40}, {717, 40}, {718, 50}, {719, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area22_Ch20[] = {{1197, 30}, {1198, 20}, {1200, 20}, {1203, 20}, {1204, 30}, {1205, 50}, {1206, 30}, {1207, 50}, {1208, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area15_Ch19[] = {{994, 30}, {995, 30}, {996, 10}, {997, 30}, {998, 25}, {999, 30}, {1000, 45}, {1001, 50}, {1002, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area16_Ch20[] = {{1017, 20}, {1018, 20}, {1021, 20}, {1024, 20}, {1025, 40}, {1026, 40}, {1027, 40}, {1028, 50}, {1029, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area02_Ch20[] = {{739, 30}, {740, 30}, {742, 30}, {745, 10}, {746, 30}, {747, 40}, {748, 30}, {749, 50}, {750, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area06_Ch20[] = {{770, 20}, {771, 20}, {773, 40}, {776, 30}, {777, 30}, {778, 20}, {779, 40}, {780, 50}, {781, 50}, {-1, 9}};
const FieldNoiseEntry NoiseEncounters_Area00_Ch20[] = {{801, 20}, {802, 40}, {804, 20}, {807, 20}, {808, 20}, {809, 40}, {810, 40}, {811, 50}, {812, 50}, {-1, 9}};
const FieldNoiseEntry* NoiseEncounters_Area16[22] = {
    NoiseEncounters_Area16_Ch00,
    NoiseEncounters_Area16_Ch01,
    NoiseEncounters_Area16_Ch02,
    NoiseEncounters_Area16_Ch03,
    NoiseEncounters_Area16_Ch04,
    NoiseEncounters_Area16_Ch05,
    NoiseEncounters_Area16_Ch06,
    NoiseEncounters_Area16_Ch07,
    NoiseEncounters_Area16_Ch08,
    NoiseEncounters_Area16_Ch09,
    NoiseEncounters_Area16_Ch10,
    NoiseEncounters_Area16_Ch11,
    NoiseEncounters_Area16_Ch12,
    NoiseEncounters_Area16_Ch13,
    NoiseEncounters_Area16_Ch14,
    NoiseEncounters_Area16_Ch15,
    NoiseEncounters_Area16_Ch16,
    NoiseEncounters_Area16_Ch17,
    NoiseEncounters_Area16_Ch18,
    NoiseEncounters_Area16_Ch19,
    NoiseEncounters_Area16_Ch20,
    NoiseEncounters_Area16_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area32[22] = {
    NoiseEncounters_Area32_Ch00,
    NoiseEncounters_Area32_Ch01,
    NoiseEncounters_Area32_Ch02,
    NoiseEncounters_Area32_Ch03,
    NoiseEncounters_Area32_Ch04,
    NoiseEncounters_Area32_Ch05,
    NoiseEncounters_Area32_Ch06,
    NoiseEncounters_Area32_Ch07,
    NoiseEncounters_Area32_Ch08,
    NoiseEncounters_Area32_Ch09,
    NoiseEncounters_Area32_Ch10,
    NoiseEncounters_Area32_Ch11,
    NoiseEncounters_Area32_Ch12,
    NoiseEncounters_Area32_Ch13,
    NoiseEncounters_Area32_Ch14,
    NoiseEncounters_Area32_Ch15,
    NoiseEncounters_Area32_Ch16,
    NoiseEncounters_Area32_Ch17,
    NoiseEncounters_Area32_Ch18,
    NoiseEncounters_Area32_Ch19,
    NoiseEncounters_Area32_Ch20,
    NoiseEncounters_Area32_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area25[22] = {
    NoiseEncounters_Area25_Ch00,
    NoiseEncounters_Area25_Ch01,
    NoiseEncounters_Area25_Ch02,
    NoiseEncounters_Area25_Ch03,
    NoiseEncounters_Area25_Ch04,
    NoiseEncounters_Area25_Ch05,
    NoiseEncounters_Area25_Ch06,
    NoiseEncounters_Area25_Ch07,
    NoiseEncounters_Area25_Ch08,
    NoiseEncounters_Area25_Ch09,
    NoiseEncounters_Area25_Ch10,
    NoiseEncounters_Area25_Ch11,
    NoiseEncounters_Area25_Ch12,
    NoiseEncounters_Area25_Ch13,
    NoiseEncounters_Area25_Ch14,
    NoiseEncounters_Area25_Ch15,
    NoiseEncounters_Area25_Ch16,
    NoiseEncounters_Area25_Ch17,
    NoiseEncounters_Area25_Ch18,
    NoiseEncounters_Area25_Ch19,
    NoiseEncounters_Area25_Ch20,
    NoiseEncounters_Area25_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area26[22] = {
    NoiseEncounters_Area26_Ch00,
    NoiseEncounters_Area26_Ch01,
    NoiseEncounters_Area26_Ch02,
    NoiseEncounters_Area26_Ch03,
    NoiseEncounters_Area26_Ch04,
    NoiseEncounters_Area26_Ch05,
    NoiseEncounters_Area26_Ch06,
    NoiseEncounters_Area26_Ch07,
    NoiseEncounters_Area26_Ch08,
    NoiseEncounters_Area26_Ch09,
    NoiseEncounters_Area26_Ch10,
    NoiseEncounters_Area26_Ch11,
    NoiseEncounters_Area26_Ch12,
    NoiseEncounters_Area26_Ch13,
    NoiseEncounters_Area26_Ch14,
    NoiseEncounters_Area26_Ch15,
    NoiseEncounters_Area26_Ch16,
    NoiseEncounters_Area26_Ch17,
    NoiseEncounters_Area26_Ch18,
    NoiseEncounters_Area26_Ch19,
    NoiseEncounters_Area26_Ch20,
    NoiseEncounters_Area26_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area07[22] = {
    NoiseEncounters_Area07_Ch00,
    NoiseEncounters_Area07_Ch01,
    NoiseEncounters_Area07_Ch02,
    NoiseEncounters_Area07_Ch03,
    NoiseEncounters_Area07_Ch04,
    NoiseEncounters_Area07_Ch05,
    NoiseEncounters_Area07_Ch06,
    NoiseEncounters_Area07_Ch07,
    NoiseEncounters_Area07_Ch08,
    NoiseEncounters_Area07_Ch09,
    NoiseEncounters_Area07_Ch10,
    NoiseEncounters_Area07_Ch11,
    NoiseEncounters_Area07_Ch12,
    NoiseEncounters_Area07_Ch13,
    NoiseEncounters_Area07_Ch14,
    NoiseEncounters_Area07_Ch15,
    NoiseEncounters_Area07_Ch16,
    NoiseEncounters_Area07_Ch17,
    NoiseEncounters_Area07_Ch18,
    NoiseEncounters_Area07_Ch19,
    NoiseEncounters_Area07_Ch20,
    NoiseEncounters_Area07_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area36[22] = {
    NoiseEncounters_Area36_Ch00,
    NoiseEncounters_Area36_Ch01,
    NoiseEncounters_Area36_Ch02,
    NoiseEncounters_Area36_Ch03,
    NoiseEncounters_Area36_Ch04,
    NoiseEncounters_Area36_Ch05,
    NoiseEncounters_Area36_Ch06,
    NoiseEncounters_Area36_Ch07,
    NoiseEncounters_Area36_Ch08,
    NoiseEncounters_Area36_Ch09,
    NoiseEncounters_Area36_Ch10,
    NoiseEncounters_Area36_Ch11,
    NoiseEncounters_Area36_Ch12,
    NoiseEncounters_Area36_Ch13,
    NoiseEncounters_Area36_Ch14,
    NoiseEncounters_Area36_Ch15,
    NoiseEncounters_Area36_Ch16,
    NoiseEncounters_Area36_Ch17,
    NoiseEncounters_Area36_Ch18,
    NoiseEncounters_Area36_Ch19,
    NoiseEncounters_Area36_Ch20,
    NoiseEncounters_Area36_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area33[22] = {
    NoiseEncounters_Area33_Ch00,
    NoiseEncounters_Area33_Ch01,
    NoiseEncounters_Area33_Ch02,
    NoiseEncounters_Area33_Ch03,
    NoiseEncounters_Area33_Ch04,
    NoiseEncounters_Area33_Ch05,
    NoiseEncounters_Area33_Ch06,
    NoiseEncounters_Area33_Ch07,
    NoiseEncounters_Area33_Ch08,
    NoiseEncounters_Area33_Ch09,
    NoiseEncounters_Area33_Ch10,
    NoiseEncounters_Area33_Ch11,
    NoiseEncounters_Area33_Ch12,
    NoiseEncounters_Area33_Ch13,
    NoiseEncounters_Area33_Ch14,
    NoiseEncounters_Area33_Ch15,
    NoiseEncounters_Area33_Ch16,
    NoiseEncounters_Area33_Ch17,
    NoiseEncounters_Area33_Ch18,
    NoiseEncounters_Area33_Ch19,
    NoiseEncounters_Area33_Ch20,
    NoiseEncounters_Area33_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area28[22] = {
    NoiseEncounters_Area28_Ch00,
    NoiseEncounters_Area28_Ch01,
    NoiseEncounters_Area28_Ch02,
    NoiseEncounters_Area28_Ch03,
    NoiseEncounters_Area28_Ch04,
    NoiseEncounters_Area28_Ch05,
    NoiseEncounters_Area28_Ch06,
    NoiseEncounters_Area28_Ch07,
    NoiseEncounters_Area28_Ch08,
    NoiseEncounters_Area28_Ch09,
    NoiseEncounters_Area28_Ch10,
    NoiseEncounters_Area28_Ch11,
    NoiseEncounters_Area28_Ch12,
    NoiseEncounters_Area28_Ch13,
    NoiseEncounters_Area28_Ch14,
    NoiseEncounters_Area28_Ch15,
    NoiseEncounters_Area28_Ch16,
    NoiseEncounters_Area28_Ch17,
    NoiseEncounters_Area28_Ch18,
    NoiseEncounters_Area28_Ch19,
    NoiseEncounters_Area28_Ch20,
    NoiseEncounters_Area28_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area12[22] = {
    NoiseEncounters_Area12_Ch00,
    NoiseEncounters_Area12_Ch01,
    NoiseEncounters_Area12_Ch02,
    NoiseEncounters_Area12_Ch03,
    NoiseEncounters_Area12_Ch04,
    NoiseEncounters_Area12_Ch05,
    NoiseEncounters_Area12_Ch06,
    NoiseEncounters_Area12_Ch07,
    NoiseEncounters_Area12_Ch08,
    NoiseEncounters_Area12_Ch09,
    NoiseEncounters_Area12_Ch10,
    NoiseEncounters_Area12_Ch11,
    NoiseEncounters_Area12_Ch12,
    NoiseEncounters_Area12_Ch13,
    NoiseEncounters_Area12_Ch14,
    NoiseEncounters_Area12_Ch15,
    NoiseEncounters_Area12_Ch16,
    NoiseEncounters_Area12_Ch17,
    NoiseEncounters_Area12_Ch18,
    NoiseEncounters_Area12_Ch19,
    NoiseEncounters_Area12_Ch20,
    NoiseEncounters_Area12_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area34[22] = {
    NoiseEncounters_Area34_Ch00,
    NoiseEncounters_Area34_Ch01,
    NoiseEncounters_Area34_Ch02,
    NoiseEncounters_Area34_Ch03,
    NoiseEncounters_Area34_Ch04,
    NoiseEncounters_Area34_Ch05,
    NoiseEncounters_Area34_Ch06,
    NoiseEncounters_Area34_Ch07,
    NoiseEncounters_Area34_Ch08,
    NoiseEncounters_Area34_Ch09,
    NoiseEncounters_Area34_Ch10,
    NoiseEncounters_Area34_Ch11,
    NoiseEncounters_Area34_Ch12,
    NoiseEncounters_Area34_Ch13,
    NoiseEncounters_Area34_Ch14,
    NoiseEncounters_Area34_Ch15,
    NoiseEncounters_Area34_Ch16,
    NoiseEncounters_Area34_Ch17,
    NoiseEncounters_Area34_Ch18,
    NoiseEncounters_Area34_Ch19,
    NoiseEncounters_Area34_Ch20,
    NoiseEncounters_Area34_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area29[22] = {
    NoiseEncounters_Area29_Ch00,
    NoiseEncounters_Area29_Ch01,
    NoiseEncounters_Area29_Ch02,
    NoiseEncounters_Area29_Ch03,
    NoiseEncounters_Area29_Ch04,
    NoiseEncounters_Area29_Ch05,
    NoiseEncounters_Area29_Ch06,
    NoiseEncounters_Area29_Ch07,
    NoiseEncounters_Area29_Ch08,
    NoiseEncounters_Area29_Ch09,
    NoiseEncounters_Area29_Ch10,
    NoiseEncounters_Area29_Ch11,
    NoiseEncounters_Area29_Ch12,
    NoiseEncounters_Area29_Ch13,
    NoiseEncounters_Area29_Ch14,
    NoiseEncounters_Area29_Ch15,
    NoiseEncounters_Area29_Ch16,
    NoiseEncounters_Area29_Ch17,
    NoiseEncounters_Area29_Ch18,
    NoiseEncounters_Area29_Ch19,
    NoiseEncounters_Area29_Ch20,
    NoiseEncounters_Area29_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area30[22] = {
    NoiseEncounters_Area30_Ch00,
    NoiseEncounters_Area30_Ch01,
    NoiseEncounters_Area30_Ch02,
    NoiseEncounters_Area30_Ch03,
    NoiseEncounters_Area30_Ch04,
    NoiseEncounters_Area30_Ch05,
    NoiseEncounters_Area30_Ch06,
    NoiseEncounters_Area30_Ch07,
    NoiseEncounters_Area30_Ch08,
    NoiseEncounters_Area30_Ch09,
    NoiseEncounters_Area30_Ch10,
    NoiseEncounters_Area30_Ch11,
    NoiseEncounters_Area30_Ch12,
    NoiseEncounters_Area30_Ch13,
    NoiseEncounters_Area30_Ch14,
    NoiseEncounters_Area30_Ch15,
    NoiseEncounters_Area30_Ch16,
    NoiseEncounters_Area30_Ch17,
    NoiseEncounters_Area30_Ch18,
    NoiseEncounters_Area30_Ch19,
    NoiseEncounters_Area30_Ch20,
    NoiseEncounters_Area30_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area19[22] = {
    NoiseEncounters_Area19_Ch00,
    NoiseEncounters_Area19_Ch01,
    NoiseEncounters_Area19_Ch02,
    NoiseEncounters_Area19_Ch03,
    NoiseEncounters_Area19_Ch04,
    NoiseEncounters_Area19_Ch05,
    NoiseEncounters_Area19_Ch06,
    NoiseEncounters_Area19_Ch07,
    NoiseEncounters_Area19_Ch08,
    NoiseEncounters_Area19_Ch09,
    NoiseEncounters_Area19_Ch10,
    NoiseEncounters_Area19_Ch11,
    NoiseEncounters_Area19_Ch12,
    NoiseEncounters_Area19_Ch13,
    NoiseEncounters_Area19_Ch14,
    NoiseEncounters_Area19_Ch15,
    NoiseEncounters_Area19_Ch16,
    NoiseEncounters_Area19_Ch17,
    NoiseEncounters_Area19_Ch18,
    NoiseEncounters_Area19_Ch19,
    NoiseEncounters_Area19_Ch20,
    NoiseEncounters_Area19_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area03[22] = {
    NoiseEncounters_Area03_Ch00,
    NoiseEncounters_Area03_Ch01,
    NoiseEncounters_Area03_Ch02,
    NoiseEncounters_Area03_Ch03,
    NoiseEncounters_Area03_Ch04,
    NoiseEncounters_Area03_Ch05,
    NoiseEncounters_Area03_Ch06,
    NoiseEncounters_Area03_Ch07,
    NoiseEncounters_Area03_Ch08,
    NoiseEncounters_Area03_Ch09,
    NoiseEncounters_Area03_Ch10,
    NoiseEncounters_Area03_Ch11,
    NoiseEncounters_Area03_Ch12,
    NoiseEncounters_Area03_Ch13,
    NoiseEncounters_Area03_Ch14,
    NoiseEncounters_Area03_Ch15,
    NoiseEncounters_Area03_Ch16,
    NoiseEncounters_Area03_Ch17,
    NoiseEncounters_Area03_Ch18,
    NoiseEncounters_Area03_Ch19,
    NoiseEncounters_Area03_Ch20,
    NoiseEncounters_Area03_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area39[22] = {
    NoiseEncounters_Area39_Ch00,
    NoiseEncounters_Area39_Ch01,
    NoiseEncounters_Area39_Ch02,
    NoiseEncounters_Area39_Ch03,
    NoiseEncounters_Area39_Ch04,
    NoiseEncounters_Area39_Ch05,
    NoiseEncounters_Area39_Ch06,
    NoiseEncounters_Area39_Ch07,
    NoiseEncounters_Area39_Ch08,
    NoiseEncounters_Area39_Ch09,
    NoiseEncounters_Area39_Ch10,
    NoiseEncounters_Area39_Ch11,
    NoiseEncounters_Area39_Ch12,
    NoiseEncounters_Area39_Ch13,
    NoiseEncounters_Area39_Ch14,
    NoiseEncounters_Area39_Ch15,
    NoiseEncounters_Area39_Ch16,
    NoiseEncounters_Area39_Ch17,
    NoiseEncounters_Area39_Ch18,
    NoiseEncounters_Area39_Ch19,
    NoiseEncounters_Area39_Ch20,
    NoiseEncounters_Area39_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area05[22] = {
    NoiseEncounters_Area05_Ch00,
    NoiseEncounters_Area05_Ch01,
    NoiseEncounters_Area05_Ch02,
    NoiseEncounters_Area05_Ch03,
    NoiseEncounters_Area05_Ch04,
    NoiseEncounters_Area05_Ch05,
    NoiseEncounters_Area05_Ch06,
    NoiseEncounters_Area05_Ch07,
    NoiseEncounters_Area05_Ch08,
    NoiseEncounters_Area05_Ch09,
    NoiseEncounters_Area05_Ch10,
    NoiseEncounters_Area05_Ch11,
    NoiseEncounters_Area05_Ch12,
    NoiseEncounters_Area05_Ch13,
    NoiseEncounters_Area05_Ch14,
    NoiseEncounters_Area05_Ch15,
    NoiseEncounters_Area05_Ch16,
    NoiseEncounters_Area05_Ch17,
    NoiseEncounters_Area05_Ch18,
    NoiseEncounters_Area05_Ch19,
    NoiseEncounters_Area05_Ch20,
    NoiseEncounters_Area05_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area35[22] = {
    NoiseEncounters_Area35_Ch00,
    NoiseEncounters_Area35_Ch01,
    NoiseEncounters_Area35_Ch02,
    NoiseEncounters_Area35_Ch03,
    NoiseEncounters_Area35_Ch04,
    NoiseEncounters_Area35_Ch05,
    NoiseEncounters_Area35_Ch06,
    NoiseEncounters_Area35_Ch07,
    NoiseEncounters_Area35_Ch08,
    NoiseEncounters_Area35_Ch09,
    NoiseEncounters_Area35_Ch10,
    NoiseEncounters_Area35_Ch11,
    NoiseEncounters_Area35_Ch12,
    NoiseEncounters_Area35_Ch13,
    NoiseEncounters_Area35_Ch14,
    NoiseEncounters_Area35_Ch15,
    NoiseEncounters_Area35_Ch16,
    NoiseEncounters_Area35_Ch17,
    NoiseEncounters_Area35_Ch18,
    NoiseEncounters_Area35_Ch19,
    NoiseEncounters_Area35_Ch20,
    NoiseEncounters_Area35_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area22[22] = {
    NoiseEncounters_Area22_Ch00,
    NoiseEncounters_Area22_Ch01,
    NoiseEncounters_Area22_Ch02,
    NoiseEncounters_Area22_Ch03,
    NoiseEncounters_Area22_Ch04,
    NoiseEncounters_Area22_Ch05,
    NoiseEncounters_Area22_Ch06,
    NoiseEncounters_Area22_Ch07,
    NoiseEncounters_Area22_Ch08,
    NoiseEncounters_Area22_Ch09,
    NoiseEncounters_Area22_Ch10,
    NoiseEncounters_Area22_Ch11,
    NoiseEncounters_Area22_Ch12,
    NoiseEncounters_Area22_Ch13,
    NoiseEncounters_Area22_Ch14,
    NoiseEncounters_Area22_Ch15,
    NoiseEncounters_Area22_Ch16,
    NoiseEncounters_Area22_Ch17,
    NoiseEncounters_Area22_Ch18,
    NoiseEncounters_Area22_Ch19,
    NoiseEncounters_Area22_Ch20,
    NoiseEncounters_Area22_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area14[22] = {
    NoiseEncounters_Area14_Ch00,
    NoiseEncounters_Area14_Ch01,
    NoiseEncounters_Area14_Ch02,
    NoiseEncounters_Area14_Ch03,
    NoiseEncounters_Area14_Ch04,
    NoiseEncounters_Area14_Ch05,
    NoiseEncounters_Area14_Ch06,
    NoiseEncounters_Area14_Ch07,
    NoiseEncounters_Area14_Ch08,
    NoiseEncounters_Area14_Ch09,
    NoiseEncounters_Area14_Ch10,
    NoiseEncounters_Area14_Ch11,
    NoiseEncounters_Area14_Ch12,
    NoiseEncounters_Area14_Ch13,
    NoiseEncounters_Area14_Ch14,
    NoiseEncounters_Area14_Ch15,
    NoiseEncounters_Area14_Ch16,
    NoiseEncounters_Area14_Ch17,
    NoiseEncounters_Area14_Ch18,
    NoiseEncounters_Area14_Ch19,
    NoiseEncounters_Area14_Ch20,
    NoiseEncounters_Area14_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area02[22] = {
    NoiseEncounters_Area02_Ch00,
    NoiseEncounters_Area02_Ch01,
    NoiseEncounters_Area02_Ch02,
    NoiseEncounters_Area02_Ch03,
    NoiseEncounters_Area02_Ch04,
    NoiseEncounters_Area02_Ch05,
    NoiseEncounters_Area02_Ch06,
    NoiseEncounters_Area02_Ch07,
    NoiseEncounters_Area02_Ch08,
    NoiseEncounters_Area02_Ch09,
    NoiseEncounters_Area02_Ch10,
    NoiseEncounters_Area02_Ch11,
    NoiseEncounters_Area02_Ch12,
    NoiseEncounters_Area02_Ch13,
    NoiseEncounters_Area02_Ch14,
    NoiseEncounters_Area02_Ch15,
    NoiseEncounters_Area02_Ch16,
    NoiseEncounters_Area02_Ch17,
    NoiseEncounters_Area02_Ch18,
    NoiseEncounters_Area02_Ch19,
    NoiseEncounters_Area02_Ch20,
    NoiseEncounters_Area02_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area06[22] = {
    NoiseEncounters_Area06_Ch00,
    NoiseEncounters_Area06_Ch01,
    NoiseEncounters_Area06_Ch02,
    NoiseEncounters_Area06_Ch03,
    NoiseEncounters_Area06_Ch04,
    NoiseEncounters_Area06_Ch05,
    NoiseEncounters_Area06_Ch06,
    NoiseEncounters_Area06_Ch07,
    NoiseEncounters_Area06_Ch08,
    NoiseEncounters_Area06_Ch09,
    NoiseEncounters_Area06_Ch10,
    NoiseEncounters_Area06_Ch11,
    NoiseEncounters_Area06_Ch12,
    NoiseEncounters_Area06_Ch13,
    NoiseEncounters_Area06_Ch14,
    NoiseEncounters_Area06_Ch15,
    NoiseEncounters_Area06_Ch16,
    NoiseEncounters_Area06_Ch17,
    NoiseEncounters_Area06_Ch18,
    NoiseEncounters_Area06_Ch19,
    NoiseEncounters_Area06_Ch20,
    NoiseEncounters_Area06_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area08[22] = {
    NoiseEncounters_Area08_Ch00,
    NoiseEncounters_Area08_Ch01,
    NoiseEncounters_Area08_Ch02,
    NoiseEncounters_Area08_Ch03,
    NoiseEncounters_Area08_Ch04,
    NoiseEncounters_Area08_Ch05,
    NoiseEncounters_Area08_Ch06,
    NoiseEncounters_Area08_Ch07,
    NoiseEncounters_Area08_Ch08,
    NoiseEncounters_Area08_Ch09,
    NoiseEncounters_Area08_Ch10,
    NoiseEncounters_Area08_Ch11,
    NoiseEncounters_Area08_Ch12,
    NoiseEncounters_Area08_Ch13,
    NoiseEncounters_Area08_Ch14,
    NoiseEncounters_Area08_Ch15,
    NoiseEncounters_Area08_Ch16,
    NoiseEncounters_Area08_Ch17,
    NoiseEncounters_Area08_Ch18,
    NoiseEncounters_Area08_Ch19,
    NoiseEncounters_Area08_Ch20,
    NoiseEncounters_Area08_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area27[22] = {
    NoiseEncounters_Area27_Ch00,
    NoiseEncounters_Area27_Ch01,
    NoiseEncounters_Area27_Ch02,
    NoiseEncounters_Area27_Ch03,
    NoiseEncounters_Area27_Ch04,
    NoiseEncounters_Area27_Ch05,
    NoiseEncounters_Area27_Ch06,
    NoiseEncounters_Area27_Ch07,
    NoiseEncounters_Area27_Ch08,
    NoiseEncounters_Area27_Ch09,
    NoiseEncounters_Area27_Ch10,
    NoiseEncounters_Area27_Ch11,
    NoiseEncounters_Area27_Ch12,
    NoiseEncounters_Area27_Ch13,
    NoiseEncounters_Area27_Ch14,
    NoiseEncounters_Area27_Ch15,
    NoiseEncounters_Area27_Ch16,
    NoiseEncounters_Area27_Ch17,
    NoiseEncounters_Area27_Ch18,
    NoiseEncounters_Area27_Ch19,
    NoiseEncounters_Area27_Ch20,
    NoiseEncounters_Area27_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area13[22] = {
    NoiseEncounters_Area13_Ch00,
    NoiseEncounters_Area13_Ch01,
    NoiseEncounters_Area13_Ch02,
    NoiseEncounters_Area13_Ch03,
    NoiseEncounters_Area13_Ch04,
    NoiseEncounters_Area13_Ch05,
    NoiseEncounters_Area13_Ch06,
    NoiseEncounters_Area13_Ch07,
    NoiseEncounters_Area13_Ch08,
    NoiseEncounters_Area13_Ch09,
    NoiseEncounters_Area13_Ch10,
    NoiseEncounters_Area13_Ch11,
    NoiseEncounters_Area13_Ch12,
    NoiseEncounters_Area13_Ch13,
    NoiseEncounters_Area13_Ch14,
    NoiseEncounters_Area13_Ch15,
    NoiseEncounters_Area13_Ch16,
    NoiseEncounters_Area13_Ch17,
    NoiseEncounters_Area13_Ch18,
    NoiseEncounters_Area13_Ch19,
    NoiseEncounters_Area13_Ch20,
    NoiseEncounters_Area13_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area17[22] = {
    NoiseEncounters_Area17_Ch00,
    NoiseEncounters_Area17_Ch01,
    NoiseEncounters_Area17_Ch02,
    NoiseEncounters_Area17_Ch03,
    NoiseEncounters_Area17_Ch04,
    NoiseEncounters_Area17_Ch05,
    NoiseEncounters_Area17_Ch06,
    NoiseEncounters_Area17_Ch07,
    NoiseEncounters_Area17_Ch08,
    NoiseEncounters_Area17_Ch09,
    NoiseEncounters_Area17_Ch10,
    NoiseEncounters_Area17_Ch11,
    NoiseEncounters_Area17_Ch12,
    NoiseEncounters_Area17_Ch13,
    NoiseEncounters_Area17_Ch14,
    NoiseEncounters_Area17_Ch15,
    NoiseEncounters_Area17_Ch16,
    NoiseEncounters_Area17_Ch17,
    NoiseEncounters_Area17_Ch18,
    NoiseEncounters_Area17_Ch19,
    NoiseEncounters_Area17_Ch20,
    NoiseEncounters_Area17_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area01[22] = {
    NoiseEncounters_Area01_Ch00,
    NoiseEncounters_Area01_Ch01,
    NoiseEncounters_Area01_Ch02,
    NoiseEncounters_Area01_Ch03,
    NoiseEncounters_Area01_Ch04,
    NoiseEncounters_Area01_Ch05,
    NoiseEncounters_Area01_Ch06,
    NoiseEncounters_Area01_Ch07,
    NoiseEncounters_Area01_Ch08,
    NoiseEncounters_Area01_Ch09,
    NoiseEncounters_Area01_Ch10,
    NoiseEncounters_Area01_Ch11,
    NoiseEncounters_Area01_Ch12,
    NoiseEncounters_Area01_Ch13,
    NoiseEncounters_Area01_Ch14,
    NoiseEncounters_Area01_Ch15,
    NoiseEncounters_Area01_Ch16,
    NoiseEncounters_Area01_Ch17,
    NoiseEncounters_Area01_Ch18,
    NoiseEncounters_Area01_Ch19,
    NoiseEncounters_Area01_Ch20,
    NoiseEncounters_Area01_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area21[22] = {
    NoiseEncounters_Area21_Ch00,
    NoiseEncounters_Area21_Ch01,
    NoiseEncounters_Area21_Ch02,
    NoiseEncounters_Area21_Ch03,
    NoiseEncounters_Area21_Ch04,
    NoiseEncounters_Area21_Ch05,
    NoiseEncounters_Area21_Ch06,
    NoiseEncounters_Area21_Ch07,
    NoiseEncounters_Area21_Ch08,
    NoiseEncounters_Area21_Ch09,
    NoiseEncounters_Area21_Ch10,
    NoiseEncounters_Area21_Ch11,
    NoiseEncounters_Area21_Ch12,
    NoiseEncounters_Area21_Ch13,
    NoiseEncounters_Area21_Ch14,
    NoiseEncounters_Area21_Ch15,
    NoiseEncounters_Area21_Ch16,
    NoiseEncounters_Area21_Ch17,
    NoiseEncounters_Area21_Ch18,
    NoiseEncounters_Area21_Ch19,
    NoiseEncounters_Area21_Ch20,
    NoiseEncounters_Area21_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area31[22] = {
    NoiseEncounters_Area31_Ch00,
    NoiseEncounters_Area31_Ch01,
    NoiseEncounters_Area31_Ch02,
    NoiseEncounters_Area31_Ch03,
    NoiseEncounters_Area31_Ch04,
    NoiseEncounters_Area31_Ch05,
    NoiseEncounters_Area31_Ch06,
    NoiseEncounters_Area31_Ch07,
    NoiseEncounters_Area31_Ch08,
    NoiseEncounters_Area31_Ch09,
    NoiseEncounters_Area31_Ch10,
    NoiseEncounters_Area31_Ch11,
    NoiseEncounters_Area31_Ch12,
    NoiseEncounters_Area31_Ch13,
    NoiseEncounters_Area31_Ch14,
    NoiseEncounters_Area31_Ch15,
    NoiseEncounters_Area31_Ch16,
    NoiseEncounters_Area31_Ch17,
    NoiseEncounters_Area31_Ch18,
    NoiseEncounters_Area31_Ch19,
    NoiseEncounters_Area31_Ch20,
    NoiseEncounters_Area31_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area15[22] = {
    NoiseEncounters_Area15_Ch00,
    NoiseEncounters_Area15_Ch01,
    NoiseEncounters_Area15_Ch02,
    NoiseEncounters_Area15_Ch03,
    NoiseEncounters_Area15_Ch04,
    NoiseEncounters_Area15_Ch05,
    NoiseEncounters_Area15_Ch06,
    NoiseEncounters_Area15_Ch07,
    NoiseEncounters_Area15_Ch08,
    NoiseEncounters_Area15_Ch09,
    NoiseEncounters_Area15_Ch10,
    NoiseEncounters_Area15_Ch11,
    NoiseEncounters_Area15_Ch12,
    NoiseEncounters_Area15_Ch13,
    NoiseEncounters_Area15_Ch14,
    NoiseEncounters_Area15_Ch15,
    NoiseEncounters_Area15_Ch16,
    NoiseEncounters_Area15_Ch17,
    NoiseEncounters_Area15_Ch18,
    NoiseEncounters_Area15_Ch19,
    NoiseEncounters_Area15_Ch20,
    NoiseEncounters_Area15_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area00[22] = {
    NoiseEncounters_Area00_Ch00,
    NoiseEncounters_Area00_Ch01,
    NoiseEncounters_Area00_Ch02,
    NoiseEncounters_Area00_Ch03,
    NoiseEncounters_Area00_Ch04,
    NoiseEncounters_Area00_Ch05,
    NoiseEncounters_Area00_Ch06,
    NoiseEncounters_Area00_Ch07,
    NoiseEncounters_Area00_Ch08,
    NoiseEncounters_Area00_Ch09,
    NoiseEncounters_Area00_Ch10,
    NoiseEncounters_Area00_Ch11,
    NoiseEncounters_Area00_Ch12,
    NoiseEncounters_Area00_Ch13,
    NoiseEncounters_Area00_Ch14,
    NoiseEncounters_Area00_Ch15,
    NoiseEncounters_Area00_Ch16,
    NoiseEncounters_Area00_Ch17,
    NoiseEncounters_Area00_Ch18,
    NoiseEncounters_Area00_Ch19,
    NoiseEncounters_Area00_Ch20,
    NoiseEncounters_Area00_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area09[22] = {
    NoiseEncounters_Area09_Ch00,
    NoiseEncounters_Area09_Ch01,
    NoiseEncounters_Area09_Ch02,
    NoiseEncounters_Area09_Ch03,
    NoiseEncounters_Area09_Ch04,
    NoiseEncounters_Area09_Ch05,
    NoiseEncounters_Area09_Ch06,
    NoiseEncounters_Area09_Ch07,
    NoiseEncounters_Area09_Ch08,
    NoiseEncounters_Area09_Ch09,
    NoiseEncounters_Area09_Ch10,
    NoiseEncounters_Area09_Ch11,
    NoiseEncounters_Area09_Ch12,
    NoiseEncounters_Area09_Ch13,
    NoiseEncounters_Area09_Ch14,
    NoiseEncounters_Area09_Ch15,
    NoiseEncounters_Area09_Ch16,
    NoiseEncounters_Area09_Ch17,
    NoiseEncounters_Area09_Ch18,
    NoiseEncounters_Area09_Ch19,
    NoiseEncounters_Area09_Ch20,
    NoiseEncounters_Area09_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area18[22] = {
    NoiseEncounters_Area18_Ch00,
    NoiseEncounters_Area18_Ch01,
    NoiseEncounters_Area18_Ch02,
    NoiseEncounters_Area18_Ch03,
    NoiseEncounters_Area18_Ch04,
    NoiseEncounters_Area18_Ch05,
    NoiseEncounters_Area18_Ch06,
    NoiseEncounters_Area18_Ch07,
    NoiseEncounters_Area18_Ch08,
    NoiseEncounters_Area18_Ch09,
    NoiseEncounters_Area18_Ch10,
    NoiseEncounters_Area18_Ch11,
    NoiseEncounters_Area18_Ch12,
    NoiseEncounters_Area18_Ch13,
    NoiseEncounters_Area18_Ch14,
    NoiseEncounters_Area18_Ch15,
    NoiseEncounters_Area18_Ch16,
    NoiseEncounters_Area18_Ch17,
    NoiseEncounters_Area18_Ch18,
    NoiseEncounters_Area18_Ch19,
    NoiseEncounters_Area18_Ch20,
    NoiseEncounters_Area18_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area04[22] = {
    NoiseEncounters_Area04_Ch00,
    NoiseEncounters_Area04_Ch01,
    NoiseEncounters_Area04_Ch02,
    NoiseEncounters_Area04_Ch03,
    NoiseEncounters_Area04_Ch04,
    NoiseEncounters_Area04_Ch05,
    NoiseEncounters_Area04_Ch06,
    NoiseEncounters_Area04_Ch07,
    NoiseEncounters_Area04_Ch08,
    NoiseEncounters_Area04_Ch09,
    NoiseEncounters_Area04_Ch10,
    NoiseEncounters_Area04_Ch11,
    NoiseEncounters_Area04_Ch12,
    NoiseEncounters_Area04_Ch13,
    NoiseEncounters_Area04_Ch14,
    NoiseEncounters_Area04_Ch15,
    NoiseEncounters_Area04_Ch16,
    NoiseEncounters_Area04_Ch17,
    NoiseEncounters_Area04_Ch18,
    NoiseEncounters_Area04_Ch19,
    NoiseEncounters_Area04_Ch20,
    NoiseEncounters_Area04_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area24[22] = {
    NoiseEncounters_Area24_Ch00,
    NoiseEncounters_Area24_Ch01,
    NoiseEncounters_Area24_Ch02,
    NoiseEncounters_Area24_Ch03,
    NoiseEncounters_Area24_Ch04,
    NoiseEncounters_Area24_Ch05,
    NoiseEncounters_Area24_Ch06,
    NoiseEncounters_Area24_Ch07,
    NoiseEncounters_Area24_Ch08,
    NoiseEncounters_Area24_Ch09,
    NoiseEncounters_Area24_Ch10,
    NoiseEncounters_Area24_Ch11,
    NoiseEncounters_Area24_Ch12,
    NoiseEncounters_Area24_Ch13,
    NoiseEncounters_Area24_Ch14,
    NoiseEncounters_Area24_Ch15,
    NoiseEncounters_Area24_Ch16,
    NoiseEncounters_Area24_Ch17,
    NoiseEncounters_Area24_Ch18,
    NoiseEncounters_Area24_Ch19,
    NoiseEncounters_Area24_Ch20,
    NoiseEncounters_Area24_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area11[22] = {
    NoiseEncounters_Area11_Ch00,
    NoiseEncounters_Area11_Ch01,
    NoiseEncounters_Area11_Ch02,
    NoiseEncounters_Area11_Ch03,
    NoiseEncounters_Area11_Ch04,
    NoiseEncounters_Area11_Ch05,
    NoiseEncounters_Area11_Ch06,
    NoiseEncounters_Area11_Ch07,
    NoiseEncounters_Area11_Ch08,
    NoiseEncounters_Area11_Ch09,
    NoiseEncounters_Area11_Ch10,
    NoiseEncounters_Area11_Ch11,
    NoiseEncounters_Area11_Ch12,
    NoiseEncounters_Area11_Ch13,
    NoiseEncounters_Area11_Ch14,
    NoiseEncounters_Area11_Ch15,
    NoiseEncounters_Area11_Ch16,
    NoiseEncounters_Area11_Ch17,
    NoiseEncounters_Area11_Ch18,
    NoiseEncounters_Area11_Ch19,
    NoiseEncounters_Area11_Ch20,
    NoiseEncounters_Area11_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area38[22] = {
    NoiseEncounters_Area38_Ch00,
    NoiseEncounters_Area38_Ch01,
    NoiseEncounters_Area38_Ch02,
    NoiseEncounters_Area38_Ch03,
    NoiseEncounters_Area38_Ch04,
    NoiseEncounters_Area38_Ch05,
    NoiseEncounters_Area38_Ch06,
    NoiseEncounters_Area38_Ch07,
    NoiseEncounters_Area38_Ch08,
    NoiseEncounters_Area38_Ch09,
    NoiseEncounters_Area38_Ch10,
    NoiseEncounters_Area38_Ch11,
    NoiseEncounters_Area38_Ch12,
    NoiseEncounters_Area38_Ch13,
    NoiseEncounters_Area38_Ch14,
    NoiseEncounters_Area38_Ch15,
    NoiseEncounters_Area38_Ch16,
    NoiseEncounters_Area38_Ch17,
    NoiseEncounters_Area38_Ch18,
    NoiseEncounters_Area38_Ch19,
    NoiseEncounters_Area38_Ch20,
    NoiseEncounters_Area38_Ch21,
};
const FieldNoiseEntry* NoiseEncounters_Area40[22] = {
    NoiseEncounters_Area40_Ch00,
    NoiseEncounters_Area40_Ch01,
    NoiseEncounters_Area40_Ch02,
    NoiseEncounters_Area40_Ch03,
    NoiseEncounters_Area40_Ch04,
    NoiseEncounters_Area40_Ch05,
    NoiseEncounters_Area40_Ch06,
    NoiseEncounters_Area40_Ch07,
    NoiseEncounters_Area40_Ch08,
    NoiseEncounters_Area40_Ch09,
    NoiseEncounters_Area40_Ch10,
    NoiseEncounters_Area40_Ch11,
    NoiseEncounters_Area40_Ch12,
    NoiseEncounters_Area40_Ch13,
    NoiseEncounters_Area40_Ch14,
    NoiseEncounters_Area40_Ch15,
    NoiseEncounters_Area40_Ch16,
    NoiseEncounters_Area40_Ch17,
    NoiseEncounters_Area40_Ch18,
    NoiseEncounters_Area40_Ch19,
    NoiseEncounters_Area40_Ch20,
    NoiseEncounters_Area40_Ch21,
};
const FieldNoiseTableSet FieldNoiseTables = {{
    NoiseEncounters_Area00,
    NoiseEncounters_Area01,
    NoiseEncounters_Area02,
    NoiseEncounters_Area03,
    NoiseEncounters_Area04,
    NoiseEncounters_Area05,
    NoiseEncounters_Area06,
    NoiseEncounters_Area07,
    NoiseEncounters_Area08,
    NoiseEncounters_Area09,
    NULL,
    NoiseEncounters_Area11,
    NoiseEncounters_Area12,
    NoiseEncounters_Area13,
    NoiseEncounters_Area14,
    NoiseEncounters_Area15,
    NoiseEncounters_Area16,
    NoiseEncounters_Area17,
    NoiseEncounters_Area18,
    NoiseEncounters_Area19,
    NULL,
    NoiseEncounters_Area21,
    NoiseEncounters_Area22,
    NULL,
    NoiseEncounters_Area24,
    NoiseEncounters_Area25,
    NoiseEncounters_Area26,
    NoiseEncounters_Area27,
    NoiseEncounters_Area28,
    NoiseEncounters_Area29,
    NoiseEncounters_Area30,
    NoiseEncounters_Area31,
    NoiseEncounters_Area32,
    NoiseEncounters_Area33,
    NoiseEncounters_Area34,
    NoiseEncounters_Area35,
    NoiseEncounters_Area36,
    NULL,
    NoiseEncounters_Area38,
    NoiseEncounters_Area39,
    NoiseEncounters_Area40,
}};
// clang-format on

const FieldNoiseEntry* const* FieldNoise_GetAreaTables(void) {
    FieldNoiseTableSet local = FieldNoiseTables;

    if ((u32)gSaveData.fieldArea >= 41) {
        return NULL;
    }
    return local.tables[gSaveData.fieldArea];
}

BOOL FieldNoise_IsKind(s32 kind, s32 encounter) {
    s32 category = Field_GetEncounterCategory(encounter);

    if (kind == 0) {
        return category < 24;
    }
    if (kind == 1) {
        return category >= 23 && category < 31;
    }
    if (kind == 2) {
        return category == 7;
    }
    return FALSE;
}

s16 FieldNoise_RollEncounter(s32 kind) {
    const FieldNoiseEntry* const* chapters = FieldNoise_GetAreaTables();
    s32                           i;
    u32                           chapter;
    s16                           roll;
    const FieldNoiseEntry*        entries;

    if (chapters == NULL) {
        return -1;
    }
    i       = 0;
    chapter = gSaveData.chapter;
    if (chapter >= 22) {
        chapter -= 22;
    }
    entries = chapters[chapter];
    roll    = RNG_Next(100);

    while (TRUE) {
        if (entries[i].encounter == -1) {
            return -1;
        }
        if (FieldNoise_IsKind(kind, entries[i].encounter)) {
            if (entries[i].weight > roll) {
                return entries[i].encounter;
            }
            roll -= entries[i].weight;
        }
        i++;
    }
}
