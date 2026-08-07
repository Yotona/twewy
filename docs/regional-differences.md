# Regional Differences

This file serves as a master list of regional differences, documenting all known locations where code differs between released editions, warranting either `#ifdef` preprocessor directives or separate source files.

## Documented Differences

The entirety of USA's overlay 36 is not present in the JP release, leaving it with 47 overlays versus USA's 48.

### src/main.c

- **[USA only](../src/main.c#L160)**: Conditional call to `func_020218ec` depending on `SystemStatusFlags.unk_03` value.

### libs/nitro/src/fx/fx_matrix.c

- **[USA only](../libs/nitro/src/fx/fx_matrix.c#L198)**: `Mtx_LookAt` function is defined.
- **[USA only](../libs/nitro/src/fx/fx_matrix.c#L333)**: `Mtx_OrthoW` function is defined.
