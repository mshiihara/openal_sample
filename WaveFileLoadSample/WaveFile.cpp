#include "WaveFile.h"

#include <stdio.h>
#include "Util.h"

WaveFile::WaveFile() {
}

WaveFile::WaveFile(const char* path) {
	open(path);
}

WaveFile::~WaveFile() {
	close();
}

bool WaveFile::open(const char* path) {
	errno_t no = fopen_s(&fp, path, "rb");
	if (no) { return false; }
	fileSize = GetFileSize(fp);
	parseHeader();
	return true;
}

void WaveFile::close() {
	if (fp) { fclose(fp); }
}

WAVEFORMATEX WaveFile::getFormat() { 
	return format; 
}

long WaveFile::getFileSize() { return fileSize; }

int WaveFile::read(void * dest, int length) {
	return fread(dest, 1, length, fp);
}

int WaveFile::parseHeader() {
	// RIFFヘッダの読み込み
	
	// RIFFチャンクは通常のチャンク情報にフォーマット情報がつく
	RIFFHeader riffHeader;
	fread(&riffHeader, sizeof(RIFFHeader), 1, fp);
	if (riffHeader.tag != CHUNK_RIFF_TAG) { return 1; }

	// ファイルポインタの保存
	long pos = ftell(fp);
	
	//fmt チャンクの取得
	WaveChunk waveChunk;
	if (parseWaveChunk(&waveChunk, CHUNK_FMT_TAG)) {
		fread(&format, sizeof(WAVEFORMATEX), 1, fp);
	}
	else { return 2; }

	fseek(fp, pos, SEEK_SET);

	//dataチャンクの取得
	waveChunk;
	if (parseWaveChunk(&waveChunk, CHUNK_DATA_TAG)) {
		return 0;
	}
	else { return 3; }
}

bool WaveFile::parseWaveChunk(WaveChunk* chunk, uint32 tag) {
	while (!feof(fp)) {
		fread(chunk, sizeof(WaveChunk), 1, fp);
		if (chunk->tag == tag) {
			return true;
		}
		else { fseek(fp, chunk->size, SEEK_CUR); }
	}
	return false;
}
