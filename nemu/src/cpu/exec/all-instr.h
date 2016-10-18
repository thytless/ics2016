#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movz.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/add.h"
#include "arith/adc.h"
#include "arith/idiv.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

#include "call/call.h"
#include "call/ret.h"
#include "call/leave.h"

#include "pushpop/push.h"
#include "pushpop/pop.h"

#include "test/test.h"

#include "jump/jbe.h"
#include "jump/jz.h"
#include "jump/jnz.h"
#include "jump/jmp.h"
#include "jump/jl.h"
#include "jump/jle.h"
#include "jump/jg.h"

#include "cmp/cmp.h"

#include "cltd/cltd.h"

#include "setcc/setnz.h"
