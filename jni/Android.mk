LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := native_assetmanager
LOCAL_MODULE_FILENAME := libassetmanager

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/include

LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := NativeAssetManager.cpp

include $(BUILD_SHARED_LIBRARY)