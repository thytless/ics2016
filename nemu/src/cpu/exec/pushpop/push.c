#include "cpu/exec/helper.h"

#define _PUSH_R_
#define DATA_BYTE 4

#define regname eax
#define SRC_REG R_EAX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ecx
#define SRC_REG R_ECX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname edx
#define SRC_REG R_EDX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ebx
#define SRC_REG R_EBX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname esp
#define SRC_REG R_ESP
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ebp
#define SRC_REG R_EBP
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname esi
#define SRC_REG R_ESI
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname edi
#define SRC_REG R_EDI
#include "push-template.h"
#undef regname
#undef SRC_REG

#undef _PUSH_R_
#undef DATA_BYTE
