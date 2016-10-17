#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE 

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE 

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE 

#define _POP_R_
#define DATA_BYTE 2

#define regname eax
#define DEST_REG R_AX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ecx
#define DEST_REG R_CX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname edx
#define DEST_REG R_DX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ebx
#define DEST_REG R_BX
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname esp
#define DEST_REG R_SP
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname ebp
#define DEST_REG R_BP
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname esi
#define DEST_REG R_SI
#include "pop-template.h"
#undef regname
#undef DEST_REG

#define regname edi
#define DEST_REG R_DI
#include "pop-template.h"
#undef regname
#undef DEST_REG
#undef DATA_BYTE

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

make_helper_v(pop_eax)
make_helper_v(pop_ecx)
make_helper_v(pop_edx)
make_helper_v(pop_ebx)
make_helper_v(pop_esp)
make_helper_v(pop_ebp)
make_helper_v(pop_esi)
make_helper_v(pop_edi)
make_helper_v(pop_rm)
