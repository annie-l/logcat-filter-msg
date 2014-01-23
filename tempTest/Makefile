all: logTest

logTest: logTest.cpp ../logcat/filter_msg.h ../logcat/filter_msg.cpp
	g++ -o logTest -g logTest.cpp ../logcat/filter_msg.cpp

testForLeaks:
	valgrind --leak-check=full -v ./logTest

clean:
	rm logTest 
