LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE := findimg
LOCAL_SRC_FILES := findimg.cpp

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)����Ϊ��̬��
#include $(BUILD_SHARED_LIBRARY)����Ϊ��̬�� 
#include $(BUILD_EXECUTABLE) ����ΪNative C��ִ�г���  
#include $(BUILD_PREBUILT) ��ģ���Ѿ�Ԥ�ȱ���

#LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT) ָ��������ɵ�ģ���Ŀ���ַ
LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT)