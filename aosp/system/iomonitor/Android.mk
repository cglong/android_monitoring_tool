LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := iomonitor.c
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE := objstr_main
include $(BUILD_EXECUTABLE)
