#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

#define _PUSH_R_

#define DATA_BYTE 2

#define regname eax
#define SRC_REG R_AX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ecx
#define SRC_REG R_CX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname edx
#define SRC_REG R_DX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ebx
#define SRC_REG R_BX
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname esp
#define SRC_REG R_SP
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname ebp
#define SRC_REG R_BP
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname esi
#define SRC_REG R_SI
#include "push-template.h"
#undef regname
#undef SRC_REG

#define regname edi
#define SRC_REG R_DI
#include "push-template.h"
#undef regname
#undef SRC_REG

#undef DATA_BYTE
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

make_helper_v(push_eax)
make_helper_v(push_ecx)
make_helper_v(push_edx)
make_helper_v(push_ebx)
make_helper_v(push_esp)
make_helper_v(push_ebp)
make_helper_v(push_esi)
make_helper_v(push_edi)
make_helper_v(push_i)
