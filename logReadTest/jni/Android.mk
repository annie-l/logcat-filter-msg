LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# give module name
LOCAL_MODULE := testLog

# List your C files to compile
LOCAL_SRC_FILES := testLog.cpp

#this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)
