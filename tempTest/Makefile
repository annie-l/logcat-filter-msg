all: logTest

logTest: logTest.cpp ../logcat/filter_msg.h ../logcat/filter_msg.cpp
	g++ -o logTest logTest.cpp ../logcat/filter_msg.cpp

clean:
	rm logTest 
