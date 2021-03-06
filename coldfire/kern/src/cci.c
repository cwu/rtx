#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "user_processes.h"
#include "msg_env_queue.h"
#include "utils.h"
#include "trace.h"
#include "k_globals.h"
#include "pong.h"
#include "trace.h"

msg_env_queue_t *msg_queue;
MsgEnv *print_env;
MsgEnv *status_env;
MsgEnv *clock_env;

void cci_intro()
{
	print_ack( "Welcome to P.OS.\r\n"
               "==========================\r\n",
               print_env, msg_queue);
}

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t status;

    char *str = k_malloc(1024);
    char *cmd = k_malloc(1024);
    void * params [11];

    msg_queue = msg_env_queue_create(); 
    print_env = request_msg_env();
    MsgEnv *proc_a_env = request_msg_env();
    status_env = request_msg_env();
    clock_env = request_msg_env();

    // Print the introduction
    cci_intro();


    //print CCI prompt
    print_ack("CCI: ", print_env, msg_queue);

    MsgEnv* env;

    while (1)
    {
        if (msg_env_queue_is_empty(msg_queue))
        {
            env = receive_message();
        }
        else
        {
            env = msg_env_queue_dequeue(msg_queue);
        }

        //envelope with characters from console
        if (env->msg_type == CONSOLE_INPUT)
        {
            rtx_strtok (env->msg, cmd, " \t\r\n");
            if ( *cmd != '\0')
            {
                //send empty envelope to process A. should only do so once.
                if (rtx_strcmp(cmd,"s") == 0) 
                {
                    if ( proc_a_env != NULL )
                    {
                        status = send_message (PROC_A_PID, proc_a_env);
                        if (status != CODE_SUCCESS)
                        {
							params[0] = &status;
                            params[1] = NULL;
							rtx_sprintf(str, "Error: could not jumpstart process A."
                                              "errno %d\r\n", params);
                            print_ack(str, print_env, msg_queue);
                        }
                        proc_a_env = NULL;
                    }
                    else
                    {
						print_ack("Process A has already been started.\r\n", print_env, msg_queue);
                    }
                }
                //displays process statuses
                else if (rtx_strcmp(cmd,"ps") == 0) 
                {
                    status = request_process_status(status_env);
                    if (status != CODE_SUCCESS)
                    {
						params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "Error: could not get the process "
                                         "statues. errno %d\r\n", params);
                        print_ack(str, print_env, msg_queue);
                    }
                    else
                    {
                        status = CCI_printProcessStatuses(status_env->msg);
                        if (status != CODE_SUCCESS)
						{
                            params[0] = &status;
                            params[1] = NULL;
                            rtx_sprintf(str,"CCI_printProcessStatuses failed "
                                            "with status %d\r\n", params);
                            print_ack(str, print_env, msg_queue);
                        }
                    }
                }
                //show clock
                else if (rtx_strcmp(cmd,"cd") == 0) 
                {
                    CCI_displayWallClock (clock_env,1);
                }
                //hide clock
                else if (rtx_strcmp(cmd,"ct") == 0)  
                {
                    CCI_displayWallClock (clock_env, 0);
                }
                //show send/receive trace buffers
                else if (rtx_strcmp(cmd,"b") == 0) 
                {
                    status = get_trace_buffers (status_env);
                    if (status != CODE_SUCCESS)
					{
                        params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "get_trace_buffers failed with status %d\r\n", params);
                        print_ack(str, print_env, msg_queue);
                    }
                    status = CCI_printTraceBuffers (status_env->msg);
                    if (status != CODE_SUCCESS)
					{					
                        params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "CCI_printTraceBuffers failed with status %d\r\n", params);
                        print_ack(str, print_env, msg_queue);
                    }
                }
                //terminate RTX
                else if (rtx_strcmp(cmd,"t") == 0)
                {
                    print_ack("Sayonara~", print_env, msg_queue);
                    int i;
                    for ( i = 0; i < 1000000; i++){}
                    terminate();
                }
                //change process priority
                else if (rtx_strcmp(cmd,"n") == 0) 
                {
                    int priority, pid, ret; 
                    char priorityStr[6], pidStr[6];
                    rtx_strtok ( NULL, priorityStr, " \t\r\n" );
                    rtx_strtok ( NULL, pidStr, " \t\r\n" );
                    ret = rtx_atoi ( priorityStr, &priority );
                    ret += rtx_atoi ( pidStr, &pid );

                    if ( ret < 2 )
                    {
						print_ack ("Error Bad command format: "
                                   "Usage: n <priority> <processID>\r\n",
                                   print_env, msg_queue);
                    }
                    else
                    {
                        status = change_priority(priority, pid);
                        if (status == ERROR_ILLEGAL_ARG)
						{
                            print_ack("Usage: n <priority> <processID>\r\n"
                                      "priority must be 0-2 and "
                                      "processID must be a user process other "
                                      "than the null process\r\n",
                                       print_env, msg_queue);
                        }
                        else if (status != CODE_SUCCESS)
						{
                            params[0] = &status;
                            params[1] = NULL;
							rtx_sprintf(str, "CCI_setNewPriority failed with status %d\r\n", params);
                            print_ack(str, print_env, msg_queue);
                        }
                    }
                }
                //set clock
                else if (rtx_strcmp(cmd,"c") == 0) 
                {
                    rtx_strtok (NULL, str, " \t\r\n");
                    if ( *str == '\0' )
                    {
						print_ack( "c\r\n"
								   "Sets the console clock.\r\n"
								   "Usage: c <hh:mm:ss>\r\n",
                                   print_env, msg_queue);
                    }
                    else
                    {
                        status = CCI_setWallClock (clock_env, str);
                    }
                }
                else if (rtx_strcmp(cmd,"pong") == 0)
                {
                    trace(ALWAYS, "pong thing started");
                    pong_mode = TRUE;
                    //init_pong();
                    //print_board();
                    MsgEnv* pongMsg = k_request_msg_env();
                    k_send_message(PONG_PID, pongMsg);
                }
                else
                {
					params[0] = env->msg;
                    params[1] = NULL;
                    rtx_sprintf(str, "Invalid command '%s'\r\n", params);
                    print_ack(str, print_env, msg_queue);
                }
            }//end if (*cmd != '\0')
            else
            {
                print_ack( cmd , print_env, msg_queue);
				print_ack( "Please enter a command.\r\n" , print_env, msg_queue);
            }
			
			print_ack( "CCI: " , print_env, msg_queue);
        }//end if env->msg_type == CONSOLE_INPUT
        else
        {
            trace_str(ERROR, "CCI: from ", k_get_process(env->send_pid)->name);
            trace_int(ERROR, "CCI: should not be getting type ", env->msg_type);
            trace_int(ERROR, "CCI: ", env->msg_type == DISPLAY_ACK);
        }
        release_msg_env(env);
    }
}
