# Makefile
# Author: Chris Wu
#

TOP_DIR = ..
MAIN_FILE = src/main.c

# Add source file name (without .c) here
OBJ_NAMES=k_api k_init k_primitives k_scheduler k_process \
		  uart_i_process msg_env_queue proc_queue proc_pq \
		  timer_i_process timeout_queue k_clock \
		  isr_handler cci cci_utils k_hotkeys k_mem k_ipc k_pong

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
COMMON_INC=rtx.h k_globals.h k_structs.h dbug.h coldfire.h
TEST_FILES=
LOCAL_CFLAGS=-D_CFSERVER_ -DTRACE_LEVEL=DEBUG
DEPS=util user

#all: $(OBJS)
include $(TOP_DIR)/shared/coldfire.mk

bin/k_api.o: $(COMMON_INC) trap_table.h 
bin/k_init.o: $(COMMON_INC) msg_env_queue.h
bin/k_primitives.o: $(COMMON_INC) k_primitives.h
bin/k_scheduler.o: $(COMMON_INC)
bin/k_process.o: $(COMMON_INC)
bin/uart_i_process.o: $(COMMON_INC) msg_env_queue.h uart_i_process.h
bin/msg_env_queue.o: $(COMMON_INC) msg_env_queue.h
bin/proc_queue.o: $(COMMON_INC) proc_queue.h
bin/proc_pq.o: $(COMMON_INC) proc_pq.h proc_queue.h
bin/timer_i_process.o: $(COMMON_INC) timer_i_process.h
bin/timeout_queue.o: $(COMMON_INC) timeout_queue.h
bin/k_clock.o: $(COMMON_INC) k_clock.h
bin/isr_handler.o: $(COMMON_INC)
bin/cci.o: $(COMMON_INC)
bin/cci_utils.o: $(COMMON_INC) msg_env_queue.h
bin/k_hotkeys.o: $(COMMON_INC) k_mem.h
bin/k_malloc.o: $(COMMON_INC) 
bin/k_ipc.o: $(COMMON_INC) k_init.h
bin/k_pong.o: $(COMMON_INC)

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

