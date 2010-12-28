LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := bin/bmlwriter
LOCAL_SRC_FILES := src/bmlwriter.c
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_STATIC_LIBRARIES += libstdc++ libc
include $(BUILD_EXECUTABLE)
