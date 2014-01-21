#ifndef __FILTER_LOG_BY_MSG_H__
#define __FILTER_LOG_BY_MSG_H__

#include <stdio.h>
#include <string.h>
#include <vector>

#define MSG_FILTER_CONFIG "/etc/logcat_msg_filters.conf"  
#define NOTHING_LOADED 	0
#define ERROR_READING_FILE -1

#define MSG_FILTER_MAX_LINE_LEN 100
#define TAG_MAX_LEN             30
#define MSG_MAX_LEN             65

using std::vector;

int is_whitespace_char( char c ) {
	if( c==' ' || c=='\t' || c=='\n' || c=='\r' ) return 1;
	else return 0;
}

typedef struct LogPriTagMsg {
	char logLevel;
	char tag [ TAG_MAX_LEN + 1 ];
	char msg [ TAG_MAX_LEN + 1 ];
} LogPriTagMsg;

int load_log_filters( int *lineNum, vector<LogPriTagMsg> &filterMsgs ) {
	// Typical filter line will be:
	// D OpenCV_NativeCamera "### Camera FPS ###"\n
	FILE *filterMsg = fopen( MSG_FILTER_CONFIG, "r" );
	if( filterMsg == 0 ) {
		if ( filterMsg == NULL) perror ("Error opening file");
		return NOTHING_LOADED;	
	}
	
	char line[ MSG_FILTER_MAX_LINE_LEN + 1 ];
	int linesRead = 0;
	LogPriTagMsg newMsg;

	while( !feof( filterMsg ) ) {
		if( fgets( line, MSG_FILTER_MAX_LINE_LEN, filterMsg ) != NULL ) {
			//fprintf( stdout,"%s", line );
			*lineNum = linesRead + 1;
			int i = 0, len = strlen( line );
			if( line[ i ] == '#' || line[ i ] =='\n' || line[ i ] =='\r' ) // Ignore line. This is a comment.
				continue;
			if( strchr("VDIWEFS",line[ i ])== NULL ) return ERROR_READING_FILE; // Incorrect type of log leve. Error! 
			newMsg.logLevel = line[ i ];// Valid type of priority level
			i++; if( !is_whitespace_char( line[ i ] ) ) return ERROR_READING_FILE; 
			while( i<len && is_whitespace_char( line[ i ] ) ) i++; 		// Gobble white spaces		
			int j=0; 
			while( i<len && !is_whitespace_char( line[i] ) ) { // Read in the tag
					newMsg.tag[ j++ ] = line[ i++ ];
					if( j >= TAG_MAX_LEN ) { 								// Exceeded max tag length. Error!
						return ERROR_READING_FILE; 
					} 
			} newMsg.tag[ j++ ] = '\0'; 
			//printf("\nTag : %s",newMsg.tag);
			if( !is_whitespace_char( line[i] ) ) return ERROR_READING_FILE; // Expected white space. Error! 	
			while( i<len && is_whitespace_char( line[ i ] ) ) i++; 		// Gobble some more white spaces	
			// We expect the match string to be specified in double quotes.
			if( line[i] != '\"' ) 
				return ERROR_READING_FILE;			// Expected double quotes. Error!
			i++; j = 0; 
			while( i<len && line[i]!='\"' ) {
				newMsg.msg[ j++ ] = line[ i++ ];
				if( j >= MSG_MAX_LEN ) return ERROR_READING_FILE;		// Exceeded max msg length. Error!
			} 
			newMsg.msg[ j ] = '\0';
			if( i+1<len-1 ) { return ERROR_READING_FILE; // Unexpected characters. Error!
			}
			//printf("\nMsg : %s\n\n",newMsg.msg);
			linesRead++;
			filterMsgs.push_back( newMsg );	
		}	
	}
	fclose( filterMsg );	
	return linesRead;
}

int android_msg_filter_should_not_printLine( const vector<LogPriTagMsg> &filters, 
	const char *tag, const char *msg ) {
	for( int i=0; i<filters.size(); i++ ) {
		if( strcmp( tag, filters[ i ].tag )==0 ) {
			if( strstr( msg, filters[ i ].msg )!= NULL ) { return 1; 
				// Found filters[i].msg in msg, so don't print the line.
			}
		}
	}
	return 0;	// Didn't find it, so print the line.	
}

#endif //  __FILTER_LOG_BY_MSG_H__
