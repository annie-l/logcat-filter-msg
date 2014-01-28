#ifndef __FILTER_LOG_BY_MSG_H__
#define __FILTER_LOG_BY_MSG_H__

#include <stdio.h>
#include <string.h>

#define MSG_FILTER_CONFIG "/system/logcat_msg_filters.conf"  
#define NOTHING_LOADED 	0
#define ERROR_READING_FILE -1

#define MSG_FILTER_MAX_LINE_LEN 100
#define TAG_MAX_LEN             30
#define MSG_MAX_LEN             65

typedef struct LogPriTagMsg {
	char logLevel;
	char tag [ TAG_MAX_LEN + 1 ];
	char msg [ MSG_MAX_LEN + 1 ];
	LogPriTagMsg *next;
	LogPriTagMsg() { next = NULL; }
	~LogPriTagMsg() {
		//printf("\nDeleting %x",next);
		delete next; 
	}
} LogPriTagMsg;

int load_log_filters( int *lineNum, LogPriTagMsg **filterMsgs );

int android_msg_filter_should_printLine( const LogPriTagMsg *filters, const char *tag, const char *msg );

#endif //  __FILTER_LOG_BY_MSG_H__
