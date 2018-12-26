#
# Copyright 2009 Cedric Priscal
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

TARGET_PLATFORM := android-3
LOCAL_LDLIBS :=-llog
LOCAL_MODULE    := libWebRtcAudio
LOCAL_SRC_FILES := WebRtcAudio.cpp

LOCAL_SRC_FILES += \
    WebRtcMoudle/analog_agc.c \
    WebRtcMoudle/complex_bit_reverse.c \
    WebRtcMoudle/complex_fft.c \
    WebRtcMoudle/copy_set_operations.c \
    WebRtcMoudle/cross_correlation.c \
    WebRtcMoudle/digital_agc.c \
    WebRtcMoudle/division_operations.c \
    WebRtcMoudle/dot_product_with_scale.c \
    WebRtcMoudle/downsample_fast.c \
    WebRtcMoudle/energy.c \
    WebRtcMoudle/fft4g.c \
    WebRtcMoudle/get_scaling_square.c \
    WebRtcMoudle/min_max_operations.c \
    WebRtcMoudle/noise_suppression.c \
    WebRtcMoudle/noise_suppression_x.c \
    WebRtcMoudle/ns_core.c \
    WebRtcMoudle/nsx_core.c \
    WebRtcMoudle/nsx_core_c.c \
    WebRtcMoudle/nsx_core_neon_offsets.c \
    WebRtcMoudle/real_fft.c \
    WebRtcMoudle/resample_48khz.c \
    WebRtcMoudle/resample_by_2.c \
    WebRtcMoudle/resample_by_2_internal.c \
    WebRtcMoudle/resample_by_2_mips.c \
    WebRtcMoudle/resample.c \
    WebRtcMoudle/resample_fractional.c \
    WebRtcMoudle/ring_buffer.c \
    WebRtcMoudle/spl_init.c \
    WebRtcMoudle/splitting_filter.c \
    WebRtcMoudle/spl_sqrt.c \
    WebRtcMoudle/spl_sqrt_floor.c \
    WebRtcMoudle/vector_scaling_operations.c

LOCAL_SHARED_LIBRARIES := libcutils libutils libc 

LOCAL_CFLAGS += -DWEBRTC_POSIX

LOCAL_C_INCLUDES += \
    WebRtcMoudle \
    $(PV_INCLUDES) \
    $(JNI_H_INCLUDE) \
    $(call include-path-for, corecg graphics)

include $(BUILD_SHARED_LIBRARY)