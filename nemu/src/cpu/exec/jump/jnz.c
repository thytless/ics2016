#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnz-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnz-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnz-template.h"
#undef DATA_BYTE

make_helper_v(jnz_i)
