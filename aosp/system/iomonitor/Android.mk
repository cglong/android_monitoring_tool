LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := iomonitor.c
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE := io_monitor_main
include $(BUILD_EXECUTABLE)
