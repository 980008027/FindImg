LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE := findimg
LOCAL_SRC_FILES := findimg.cpp

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)编译为静态库
#include $(BUILD_SHARED_LIBRARY)编译为动态库 
#include $(BUILD_EXECUTABLE) 编译为Native C可执行程序  
#include $(BUILD_PREBUILT) 该模块已经预先编译

#LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT) 指定最后生成的模块的目标地址
LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT)