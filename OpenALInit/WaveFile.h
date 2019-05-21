#pragma once

#include <stdio.h>
#include <Windows.h>
#include "type.h"

class WaveFile {
private:
	// �`�����N�����ʂ���ׂ̒l
	static const uint32 CHUNK_RIFF_TAG = 'FFIR';
	static const uint32 CHUNK_FMT_TAG  = ' tmf';
	static const uint32 CHUNK_DATA_TAG = 'atad';

	// RIFF�`�����N�͒ʏ�̃`�����N���Ƀt�H�[�}�b�g��񂪂�
	struct RIFFHeader{
		uint32 tag;
		uint32 size;
		uint32 format;
	};

	// Wave�t�@�C���̃`�����N�����i�[���邽�߂̌^
	struct WaveChunk {
		uint32 tag;
		uint32 size;
	};

	FILE* fp = nullptr;
	WAVEFORMATEX format;
	long fileSize = -1;
	long dataSize = -1;
public:
	WaveFile();
	WaveFile(const char* path);
	virtual ~WaveFile();

	bool open(const char* path);
	void close();

	WAVEFORMATEX const * getFormat();
	long getFileSize();
	long getDataSize();
	int read(void* dest, int length);

private:
	int parseHeader();
	bool parseWaveChunk(WaveChunk* chunk, uint32 tag);
};