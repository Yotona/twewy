#include <MSL/ARM/critical_regions.h>

OSMutex __cs[num_critical_regions];
int     __cs_id[num_critical_regions];
int     __cs_ref[num_critical_regions];
