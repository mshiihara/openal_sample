#include <Windows.h>
#include <stdio.h>
#include <vorbis/vorbisfile.h>
#include <al.h>
#include <alc.h>

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "winmm.lib")

int main(void) {
	OggVorbis_File ovf;
	if (ov_fopen("se_maoudamashii_battle13.ogg", &ovf)) {
		printf("ogg file open failed\n");
	}

	vorbis_info* oggInfo = ov_info(&ovf, -1);

	printf("version:%d\n",         oggInfo->version);
	printf("channels:%d\n",        oggInfo->channels);
	printf("rate:%d\n",            oggInfo->rate);
	printf("bitrate_upper:%d\n",   oggInfo->bitrate_upper);
	printf("bitrate_nominal:%d\n", oggInfo->bitrate_nominal);
	printf("bitrate_lower:%d\n",   oggInfo->bitrate_lower);	
	printf("bitrate_window:%d\n",  oggInfo->bitrate_window);

	// バッファ
	ogg_int64_t  size = ov_pcm_total(&ovf, -1) * oggInfo->channels * 2;

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
	// ステレオとモノラルを判別
	//
	ALenum format;
	// モノラル
	if (oggInfo->channels == 1) {
		format = AL_FORMAT_MONO16;
	}
	// ステレオ
	else {
		format = AL_FORMAT_STEREO16;
	}

	//
	// バッファに波形データを転送しソースと接続
	//
	byte* buffer = new byte[size];
	int bitstream = -1;
	long readByte = 0;
	while (readByte < size) {
		readByte += ov_read(&ovf, (char*)(buffer + readByte), size, 0, 2, 1, &bitstream);
	}
	alBufferData(bufferId, format, buffer, size, oggInfo->rate);
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

	ov_clear(&ovf);

	getchar();
	return 0;
}