LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_OWNER := qcom
LOCAL_PROPRIETARY_MODULE := true
LOCAL_SRC_FILES := volume.cpp

LOCAL_MODULE := mmi_volume
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -Wall

LOCAL_C_INCLUDES := $(MMI_ROOT)/libmmi

LOCAL_C_INCLUDES += external/connectivity/stlport/stlport
LOCAL_C_INCLUDES += bootable/recovery/minui

LOCAL_SHARED_LIBRARIES := libmmi libminui libcutils

LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

include $(BUILD_SHARED_LIBRARY)

