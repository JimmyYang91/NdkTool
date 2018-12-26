#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include "WebRtcMoudle/noise_suppression.h"
#include "WebRtcMoudle/noise_suppression_x.h"
#include "WebRtcMoudle/signal_processing_library.h"
#include "WebRtcMoudle/gain_control.h"


#ifdef __cplusplus
extern "C" {
#endif


FILE *infp      = NULL;
FILE *outfp     = NULL;

short *pData    = NULL;
short *pOutData = NULL;
void *agcHandle = NULL;
int inMicLevel  = 0;
int outMicLevel = 0;
int micLevelIn = 0;
int micLevelOut = 0;

//agc:
void Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcAgcInit(JNIEnv *env, jobject object, jlong minLevel, jlong maxLevel, jlong fs){
    int agcMode  = kAgcModeFixedDigital;
    WebRtcAgc_Create(&agcHandle);
    WebRtcAgc_Init(agcHandle, (int32_t)minLevel, (int32_t)maxLevel, (int16_t)agcMode, (uint32_t)fs);
    WebRtcAgc_config_t agcConfig;
    agcConfig.compressionGaindB = 23;//20;
    agcConfig.limiterEnable     = 1;
    agcConfig.targetLevelDbfs   = 3;
    WebRtcAgc_set_config(agcHandle, agcConfig);
};


void Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcAgcFun(JNIEnv *env, jobject object,jshortArray inAudio, jshortArray outAudio ,jlong frameSize)
{

    do
    {
        //int16_t frameSize = 80;
        pData    = env->GetShortArrayElements(inAudio, NULL);
        pOutData = env->GetShortArrayElements(outAudio, NULL);
        size_t len = (int16_t)frameSize*sizeof(short);
        inMicLevel  = micLevelOut;
        outMicLevel = 0;
        uint8_t saturationWarning;
        WebRtcAgc_Process(agcHandle, pData, NULL, frameSize, pOutData,NULL, inMicLevel, &outMicLevel, 0, &saturationWarning);
        micLevelIn = outMicLevel;
        env->ReleaseShortArrayElements(inAudio, pData, NULL);
        env->ReleaseShortArrayElements(outAudio, pOutData, NULL);
    } while (0);
}

void Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcAgcFree(JNIEnv *env, jobject object)
{
    WebRtcAgc_Free(agcHandle);
}
///// agc end /////

//noise supresstion:
JNIEXPORT jint JNICALL Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcNsInit
  (JNIEnv *env, jobject obj, jint sample_rate){

	int ret ;
	NsHandle* NS_instance;

	if ((ret = WebRtcNs_Create(&NS_instance) )) {
		return ret;
	}
	if ((ret = WebRtcNs_Init(NS_instance, sample_rate) )) {
		return ret;
	}

	if ( ( ret =  WebRtcNs_set_policy(NS_instance, 2) ) ){
		return ret;
	}

	return (jint)NS_instance;
}

JNIEXPORT jshortArray JNICALL Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcNsProcess
  (JNIEnv *env, jobject obj, jint handle, jshortArray input_frame  ){
	//get handle
	NsHandle* NS_instance = (NsHandle* )handle;

	//create native shorts from java shorts
	jshort *native_input_frame = env->GetShortArrayElements(input_frame, NULL);

	//allocate memory for output data
	jint length = env->GetArrayLength(input_frame);
	jshortArray temp = env->NewShortArray(length);
	jshort *native_output_frame = env->GetShortArrayElements(temp, 0);

	//noise suppression
	if(
		WebRtcNs_Process(NS_instance ,native_input_frame ,NULL ,native_output_frame , NULL) ||
	WebRtcNs_Process(NS_instance ,&native_input_frame[80] ,NULL ,&native_output_frame[80] , NULL) ){
		return NULL;
	}

	/*
	 * int WebRtcNs_Process(NsHandle* NS_inst,
                     short* spframe,
                     short* spframe_H,
                     short* outframe,
                     short* outframe_H);
                     */
	//ret = WebRtcNs_Process(denoiser->NS_inst, pAudioFrame, tsk_null, denoiser->temp_rec_out, tsk_null)

	//convert native output to java layer output
	jshortArray output_shorts = env->NewShortArray(length);
	env->SetShortArrayRegion(output_shorts, 0, length, native_output_frame);

	//cleanup and return
	env->ReleaseShortArrayElements(input_frame, native_input_frame, 0);
	env->ReleaseShortArrayElements(temp, native_output_frame, 0);

	env->DeleteLocalRef(temp);
	return output_shorts;

}

JNIEXPORT jint JNICALL Java_com_example_ipmobile_audio_WebRtcAudio_WebRtcNsFree
  (JNIEnv *env, jobject obj, jint handle){
	return 0;
}
///// ns end /////


#ifdef __cplusplus
}

#endif