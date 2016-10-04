#include "cpu/exec/helper.h"

#define _POP_R_
#define DATA_BYTE 4

#define regname eax
#define DEST_REG R_EAX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ecx
#define DEST_REG R_ECX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname edx
#define DEST_REG R_EDX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ebx
#define DEST_REG R_EBX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname esp
#define DEST_REG R_ESP
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ebp
#define DEST_REG R_EBP
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname esi
#define DEST_REG R_ESI
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname edi
#define DEST_REG R_EDI
#include "pop-template.h"
#undef regname
#undef DEST_REG

#undef _POP_R_
#undef DATA_BYTE
