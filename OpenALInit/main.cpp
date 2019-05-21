#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

#include "WaveFile.h"


int main(void) {
	

	//
	// OpenAL関連の変数
	//
	ALCcontext* context = nullptr;
	ALuint bufferId = 0;
	ALuint sourceId = 0;
	
	//
	// OpenALを初期化
	//
	ALCdevice* device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	//
	// バッファとソースを生成
	//
	alGenBuffers(1, &bufferId);
	alGenSources(1, &sourceId);

	//
	// Waveファイルの読み込み
	//
	WaveFile waveFile("magical30.wav");

	//
	// ステレオとモノラルを判別
	//
	ALenum format;
	// モノラル
	if (waveFile.getFormat()->nChannels == 1) {
		if (waveFile.getFormat()->wBitsPerSample == 8) { format = AL_FORMAT_MONO8; }
		else { format = AL_FORMAT_MONO16; }
	}
	// ステレオ
	else {
		if (waveFile.getFormat()->wBitsPerSample == 8) { format = AL_FORMAT_STEREO8; }
		else { format = AL_FORMAT_STEREO16; }
	}
	
	//
	// バッファに波形データを転送しソースと接続
	//
	byte* buffer = new byte[waveFile.getDataSize()];
	waveFile.read(buffer, waveFile.getDataSize());
	alBufferData(bufferId, format, buffer, waveFile.getDataSize(), waveFile.getFormat()->nSamplesPerSec);
	delete[] buffer;

	// ソースオブジェクトにオーディオバッファを設定
	alSourcei(sourceId, AL_BUFFER, bufferId);

	//
	// 再生
	//
	alSourcePlay(sourceId);
	printf("終了するには何かキーを押してください\n");
	getchar();

	//
	// 再生と停止しソースオブジェクトとオーディオバッファを破棄
	//
	alSourceStop(bufferId);
	alDeleteSources(1, &bufferId);
	alDeleteBuffers(1, &bufferId);

	//
	// OpenALを破棄
	//
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}