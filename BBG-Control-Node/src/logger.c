/**
 * @file logger.c
 * @author Siddhant Jajoo
 * @brief This file consists of the logger function which prints the data to the logfile.
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "logger.h"

bool previous_state;

/**
 * @brief - This function logs data to the textfile depending on the id field obtained from the structure sensor_struct
 * 			upon dequeuing the data.
 * 
 * @param data_rcv - This is the local object of the structure sensor_struct. This is obtained from function queue_receive().
  
 */
void log_data(logger_struct data_rcv)
{
	switch (data_rcv.id)
	{

	case ERROR_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.msg.error_data.data_time.tv_sec, data_rcv.msg.error_data.data_time.tv_nsec);
		fprintf(stdout, "%s.\n", data_rcv.msg.error_data.error_str);
		fprintf(stdout, "%s.\n", strerror(data_rcv.msg.error_data.error_value));
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.msg.error_data.data_time.tv_sec, data_rcv.msg.error_data.data_time.tv_nsec);
		fprintf(logfile, "%s.\n", data_rcv.msg.error_data.error_str);
		fprintf(logfile, "%s.\n", strerror(data_rcv.msg.error_data.error_value));
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		break;
	}

	case MSG_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout,"[%lu sec, %lu nsec] ",data_rcv.msg.msg_data.data_time.tv_sec, data_rcv.msg.msg_data.data_time.tv_nsec);
		fprintf(stdout, "%s", data_rcv.msg.msg_data.msg_str);
		fprintf(logfile,"[%lu sec, %lu nsec] ",data_rcv.msg.msg_data.data_time.tv_sec, data_rcv.msg.msg_data.data_time.tv_nsec);
		fprintf(logfile, "%s", data_rcv.msg.msg_data.msg_str);
		fclose(logfile);
		break;
	}
	case MSG_RCV_REMOTE_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout,"[%lu sec, %lu nsec] ",data_rcv.msg.msg_data.data_time.tv_sec, data_rcv.msg.msg_data.data_time.tv_nsec);
		fprintf(stdout, "[REMOTE NODE]: %s\n", data_rcv.msg.msg_data.msg_str);
		fprintf(logfile,"[%lu sec, %lu nsec] ",data_rcv.msg.msg_data.data_time.tv_sec, data_rcv.msg.msg_data.data_time.tv_nsec);
		fprintf(logfile, "[REMOTE_NODE]: %s\n", data_rcv.msg.msg_data.msg_str);
		fclose(logfile);
		break;
	}
	default:
		break;
	}
}
