LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE := libiostats
include $(BUILD_SHARED_LIBRARY)
