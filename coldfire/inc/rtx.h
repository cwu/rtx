#ifndef _RTX_H_
#define _RTX_H_

#include "coldfire.h"

#define CODE_SUCCESS        0
#define ERROR_NULL_ARG      -1
#define ERROR_ILLEGAL_ARG   -2
#define ERROR_ERROR_ARG     -3

#define IPC_MESSAGE_TRACE_HISTORY_SIZE 16

#define DISPLAY_ACK    -1
#define CONSOLE_OUTPUT -2
#define CONSOLE_OUTPUT_REQUEST_ACK -3
#define CONSOLE_INPUT  -4
#define REQUEST_CHAR   -5
#define COUNT_REPORT   -6

// wakeup codes
#define WAKEUP_10       -10

typedef void (*start_pc)();
typedef struct MsgEnv {
    struct MsgEnv *next;
    int32_t dest_pid;
    int32_t send_pid;
    int32_t  msg_type;
    char *   msg;
} MsgEnv;

typedef struct ipc_trace {
    int32_t dest_pid;
    int32_t send_pid;
    int32_t msg_type;
    uint32_t time_stamp;
} ipc_trace_t;

enum process_state {
    P_READY, P_EXECUTING, P_BLOCKED_ON_ENV_REQUEST, P_BLOCKED_ON_RECEIVE, 
    P_INTERRUPTED
};

typedef struct process_status {
    int32_t    pid;
    enum process_state  status;
    uint32_t priority;
} process_status_t;

/** 5.1 Interprocess Communication **/
int send_message(int dest_pid, MsgEnv *msg_env);
MsgEnv * receive_message();

/** 5.2 Storage Management **/
MsgEnv * request_msg_env();
int release_msg_env(MsgEnv * msg_env);

/** 5.3 Processor Management **/
int release_processor();
int request_process_status(MsgEnv *msg_env);
int terminate();
int change_priority(int new_priority, int target_process_id);

/** 5.4 Timing Servicies **/
int request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env);

/** 5.5 System Console I/O **/
int send_console_chars(MsgEnv *msg_env, bool request_ack);

/** 5.6 Interprocess Message Trace **/
int get_trace_buffers(MsgEnv* msg_env);

uint32_t get_system_time();

#endif
