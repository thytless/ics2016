##### global settings #####

.PHONY: nemu entry testcase kernel run gdb test submit clean

CC := gcc
LD := ld
CFLAGS := -MMD -Wall -Werror -c

LIB_COMMON_DIR := lib-common
LIBC_INC_DIR := $(LIB_COMMON_DIR)/uclibc/include
LIBC_LIB_DIR := $(LIB_COMMON_DIR)/uclibc/lib
LIBC := $(LIBC_LIB_DIR)/libc.a
FLOAT := obj/$(LIB_COMMON_DIR)/FLOAT/FLOAT.a

include config/Makefile.git
include config/Makefile.build

all: nemu


##### rules for building the project #####

include nemu/Makefile.part
include testcase/Makefile.part
include lib-common/FLOAT/Makefile.part
include kernel/Makefile.part
include game/Makefile.part

nemu: $(nemu_BIN)
testcase: $(testcase_BIN)
kernel: $(kernel_BIN)
game: $(game_BIN)


##### rules for cleaning the project #####

clean-nemu:
	-rm -rf obj/nemu 2> /dev/null

clean-testcase:
	-rm -rf obj/testcase 2> /dev/null

clean-kernel:
	-rm -rf obj/kernel 2> /dev/null

clean-game:
	-rm -rf obj/game 2> /dev/null

clean: clean-cpp
	-rm -rf obj 2> /dev/null
	-rm -f *log.txt entry $(FLOAT) 2> /dev/null


##### some convinient rules #####
		
USERPROG := obj/testcase/mov-c							 
#USERPROG := obj/testcase/wanshu						 
#USERPROG := obj/testcase/bubble-sort					 
#USERPROG := obj/testcase/add							 
#USERPROG := obj/testcase/add-longlong					 
#USERPROG := obj/testcase/bit							 
#USERPROG := obj/testcase/fact							 
#USERPROG := obj/testcase/fib							 
#USERPROG := obj/testcase/gotbaha						 
#USERPROG := obj/testcase/if-else						
#USERPROG := obj/testcase/leap-year				
#USERPROG := obj/testcase/matrix-mul-small				 
#USERPROG := obj/testcase/matrix-mul					 
#USERPROG := obj/testcase/max
#USERPROG := obj/testcase/min3					
#USERPROG := obj/testcase/movsx					
#USERPROG := obj/testcase/mul-longlong
#USERPROG := obj/testcase/pascal						 
#USERPROG := obj/testcase/prime							 
#USERPROG := obj/testcase/sum
#USERPROG := obj/testcase/switch				
#USERPROG := obj/testcase/quick-sort
#USERPROG := obj/testcase/select-sort

#USERPROG := obj/testcase/shuixianhua			
#USERPROG := obj/testcase/sub-longlong
#USERPROG := obj/testcase/to-lower-case

#USERPROG := obj/testcase/struct
#USERPROG := obj/testcase/string
#USERPROG := obj/testcase/hello-str


# n USERPROG := obj/testcase/hello
# n USERPROG := obj/testcase/hello-inline-asm
#USERPROG := obj/testcase/integral
#USERPROG := obj/testcase/quadratic-eq
#USERPROG := obj/testcase/print-FLOAT
#USERPROG := obj/testcase/print-FLOAT-linux

#ENTRY := $(USERPROG)
ENTRY := $(kernel_BIN)

entry: $(ENTRY)
	objcopy -S -O binary $(ENTRY) entry

run: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "run")
	$(nemu_BIN) $(USERPROG)

gdb: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "gdb")
	gdb -s $(nemu_BIN) --args $(nemu_BIN) $(USERPROG)

test: $(nemu_BIN) $(testcase_BIN) entry
	$(call git_commit, "test")
	bash test.sh $(testcase_BIN)

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

count:
	find . -name "*.[c|h]"| xargs grep '^.' | wc -l
