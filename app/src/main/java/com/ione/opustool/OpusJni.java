package com.ione.opustool;
/**
 * 压缩库
 *
 * @author lzhen
 *
 */
public class OpusJni
{
	private static OpusJni mInstance;

	public synchronized static OpusJni getInstance() {
		if (mInstance == null)
			mInstance = new OpusJni();
		return mInstance;
	}

	static
	{
		System.loadLibrary("opus");
	}
	// Decoder
	public native long opusDecoderCreate(int sampleRate, int channels);
	public native int opusDecodeFloat(long decoder, byte[] data, int length, float[] pcm, int frameSize, int decodeFec);
	public native void opusDecoderDestroy(long decoder);
	// Encoder
	public native long opusEncoderCreate(int sampleRate, int channels, int application);
	public native int opusEncoderCtl(long encoder, int request, int value);
	public native int opusEncode(long encoder, short[] pcm, int frameSize, byte[] data, int maxBytes);
	public native void opusEncoderDestroy(long encoder);
	// General
	public native int opusPacketGetFrames(byte[] data, int length);
	public native int opusPacketGetSamplesPerFrame(byte[] data, int sampleRate);
	public native int opusPacketGetChannels(byte[] data);
}