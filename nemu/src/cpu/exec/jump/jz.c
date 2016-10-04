#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jz-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jz-template.h"
#undef DATA_BYTE


