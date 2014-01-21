#include "../../logcat/filter_msg.h"

int main( int argc, char *argv[] ) {
	int lineNum;
	LogPriTagMsg *filters = NULL;	
	int success = load_log_filters( &lineNum, &filters );
	if( success >= 0 ) {
		printf( "\nLoad successful. Lines Read =%d\n", success );
	}
	else {
		printf( "\nLoad unsuccessful. Returned %d.\n", success );
		printf( "\nError on line %d\n", lineNum );
	}
	
	// Display all messages read in now.
	LogPriTagMsg *filter = filters;
	while( filter!= NULL ) {
		printf( "\nLog level: %c, Tag: %s, Message: %s\n", filter->logLevel, filter->tag, filter->msg );
		filter = filter->next;
	}

	printf( "\n\n\nNow testing filtering\n" );
	
	char str1[] = "D/NvOsDebugPrintf(23187): CALIBRATION STATUS not CLEARED!";		
	char str2[] = "D/OpenCV_NativeCamera(16874): CameraHandler::initCameraConnect(0x694304f5, 0, 0x67407d70, 0x0)";
	char str3[] = "I/OpenCV_NativeCamera(16874): ### Camera FPS ### [53520] Frames, 30.01 FPS"
;
	if( android_msg_filter_should_not_printLine( filters,"NvOsDebugPrintf", str1 ) ) printf("\nWill not print str1");
	if( android_msg_filter_should_not_printLine( filters,"OpenCV_NativeCamera", str2 ) ) printf("\nWill not print str2");
	if( android_msg_filter_should_not_printLine( filters,"OpenCV_NativeCamera", str3 ) ) printf("\nWill not print str3");

	printf( "\n\n" );

}
