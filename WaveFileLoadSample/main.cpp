#include "WaveFile.h"

#pragma comment(lib, "winmm.lib")

// 再生用バッファの関連定数
#define BUFFER_LEN 44100
#define BUFFER_NUM 2

// オーディオデバイスのハンドル
HWAVEOUT hWaveOut;

// バッファ
WAVEHDR whdr[BUFFER_NUM];

WaveFile waveFile("Electro_1.wav");

void CALLBACK waveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	static int bn = 0;
	if (uMsg == WOM_DONE) {
		//バッファを読み込み
		whdr[bn % 2].dwBufferLength = waveFile.read(whdr[bn % 2].lpData, BUFFER_LEN);
		//読み込んだバッファをキューへ
		waveOutWrite(hWaveOut, &whdr[bn % 2], sizeof(WAVEHDR));
		// バッファのインデックスを切り替え
		bn++;
	}
}

int main(void) {
	
	//波形を出力するオーディオデバイスを開く
	MMRESULT r = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFile.getFormat(), (DWORD)waveCallback, (DWORD)0, CALLBACK_FUNCTION);
	
	// バッファを確保
	for (int i = 0; i < BUFFER_NUM; i++) {
		whdr[i].lpData = (LPSTR)malloc(BUFFER_LEN);
		whdr[i].dwBytesRecorded = 0;
		whdr[i].dwUser          = 0;
		whdr[i].dwFlags         = 0;
		whdr[i].dwLoops         = 0;
		whdr[i].lpNext          = 0;
		whdr[i].reserved        = 0;
		waveOutPrepareHeader(hWaveOut, &whdr[i], sizeof(WAVEHDR));
	}

	// バッファにデータを読み込む
	for (int i = 0; i < BUFFER_NUM; i++) {
		whdr[i].dwBufferLength = waveFile.read(whdr[i].lpData, BUFFER_LEN);
	}

	// 波形データをオーディオデバイスに書き込み。書き込み後、直ちに波形は再生される。
	for (int i = 0; i < BUFFER_NUM; i++) {
		waveOutWrite(hWaveOut, &whdr[i], sizeof(WAVEHDR));
	}

	printf("終了するには何かキーを押してください\n");
	getchar();

	// オーディオデバイスを閉じる
	waveOutClose(hWaveOut);

	// waveデータを解放
	for (int i = 0; i < BUFFER_NUM; i++) {
		free(whdr[i].lpData);
	}

	return -1;
}