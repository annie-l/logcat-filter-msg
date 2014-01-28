#include "filter_msg.h"
#include <stdio.h>
#include <ctype.h> // isspace(c)

/*int is_whitespace_char( char c ) {
	if( c==' ' || c=='\t' || c=='\n' || c=='\r' ) return 1;
	else return 0;
}*/

int cleanUpAndReturn( LogPriTagMsg *newMsg, FILE *filterFile, int errorCode ) {
	delete newMsg;
	fclose( filterFile );
	return errorCode;
}

int load_log_filters( int *lineNum, LogPriTagMsg **filterMsgs ) {
	// Typical filter line will be:
	// D "OpenCV_NativeCamera" "### Camera FPS ###"\n
	// delete *filterMsgs;
	*filterMsgs = NULL;
	LogPriTagMsg *curr = *filterMsgs;

	FILE *filterFile = fopen( MSG_FILTER_CONFIG, "r" );
	if ( filterFile == NULL) { 
		perror ("Error opening file");
		return NOTHING_LOADED;	
	}
	char line[ MSG_FILTER_MAX_LINE_LEN + 1 ];
	int linesRead = 0;

	while( fgets( line, MSG_FILTER_MAX_LINE_LEN, filterFile ) ) {
			//fprintf( stdout,"%s", line );
			*lineNum = linesRead + 1;
			int i = 0;
			if( line[ i ] == '#' || line[ i ] =='\n' || line[ i ] =='\r' ) // Ignore line. This is a comment.
				continue;
 			int len = strlen( line );
			
			LogPriTagMsg *newMsg = new LogPriTagMsg(); newMsg->next = NULL;
			// Read in log priority level.
			if( strchr("VDIWEFS",line[ i ])== NULL ) {
				return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE ); } // Incorrect type of log level. Error! 
			newMsg->logLevel = line[ i ]; // Valid type of priority level

			// Read in tag.
			i++; if( !isspace( line[ i ] ) ) return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE ); 
			while( i<len && isspace( line[ i ] ) ) i++; // Gobble white spaces		
			int j=0; 
			while( (i+1)<len && !(isspace( line[i] ) && line[i+1]=='\"') ) { // Read in the tag
					newMsg->tag[ j++ ] = line[ i++ ];
					if( j >= TAG_MAX_LEN ) { 								// Exceeded max tag length. Error!
						return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE ); 
					} 
			} newMsg->tag[ j++ ] = '\0'; 
			//printf("\nAddress: %x, \nTag : %s,",newMsg, newMsg->tag);

			// Read in msg.
			if( !isspace( line[i] ) ) return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE ); // Expected white space. Error! 	
			while( i<len && isspace( line[ i ] ) ) i++; 		// Gobble some more white spaces	
			// We expect the match string to be specified in double quotes.
			if( line[i] != '\"' ) 
				return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE );			// Expected double quotes. Error!
			i++; j = 0; 
			while( i<len && line[i]!='\"' ) {
				newMsg->msg[ j++ ] = line[ i++ ];
				if( j >= MSG_MAX_LEN ) return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE );		// Exceeded max msg length. Error!
			} 
			newMsg->msg[ j ] = '\0';
			if( i+1<len-1 ) { return cleanUpAndReturn( newMsg, filterFile, ERROR_READING_FILE ); // Unexpected characters. Error!
			}
			//printf("\nMsg : %s\n\n",newMsg->msg);
			linesRead++;

			if( (*filterMsgs)==NULL ) { 
				(*filterMsgs) = newMsg; 
				curr = newMsg;
			}
			else { // Create a new message.
				curr->next = newMsg; 	
				curr = curr->next;
			}
	}	
	fclose( filterFile );	
	return linesRead;
}

int android_msg_filter_should_printLine( const LogPriTagMsg *filters, 
	const char *tag, const char *msg ) {
	LogPriTagMsg *filter = (LogPriTagMsg*)filters;
	while( filter != NULL ) {
		if( strstr( tag, filter->tag )!= NULL ) {
			if( strstr( msg, filter->msg )!= NULL ) { return 0; 
				// Found filters[i].msg in msg, so don't print the line.
			}
		}
		filter = filter->next;
	}
	return 1;	// Didn't find it, so print the line.	
}
