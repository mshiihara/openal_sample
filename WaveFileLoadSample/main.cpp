#include "WaveFile.h"

#pragma comment(lib, "winmm.lib")

// �Đ��p�o�b�t�@�̊֘A�萔
#define BUFFER_LEN 44100
#define BUFFER_NUM 2

// �I�[�f�B�I�f�o�C�X�̃n���h��
HWAVEOUT hWaveOut;

// �o�b�t�@
WAVEHDR whdr[BUFFER_NUM];

WaveFile waveFile("Electro_1.wav");

void CALLBACK waveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	static int bn = 0;
	if (uMsg == WOM_DONE) {
		//�o�b�t�@��ǂݍ���
		whdr[bn % 2].dwBufferLength = waveFile.read(whdr[bn % 2].lpData, BUFFER_LEN);
		//�ǂݍ��񂾃o�b�t�@���L���[��
		waveOutWrite(hWaveOut, &whdr[bn % 2], sizeof(WAVEHDR));
		// �o�b�t�@�̃C���f�b�N�X��؂�ւ�
		bn++;
	}
}

int main(void) {
	
	//�g�`���o�͂���I�[�f�B�I�f�o�C�X���J��
	MMRESULT r = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFile.getFormat(), (DWORD)waveCallback, (DWORD)0, CALLBACK_FUNCTION);
	
	// �o�b�t�@���m��
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

	// �o�b�t�@�Ƀf�[�^��ǂݍ���
	for (int i = 0; i < BUFFER_NUM; i++) {
		whdr[i].dwBufferLength = waveFile.read(whdr[i].lpData, BUFFER_LEN);
	}

	// �g�`�f�[�^���I�[�f�B�I�f�o�C�X�ɏ������݁B�������݌�A�����ɔg�`�͍Đ������B
	for (int i = 0; i < BUFFER_NUM; i++) {
		waveOutWrite(hWaveOut, &whdr[i], sizeof(WAVEHDR));
	}

	printf("�I������ɂ͉����L�[�������Ă�������\n");
	getchar();

	// �I�[�f�B�I�f�o�C�X�����
	waveOutClose(hWaveOut);

	// wave�f�[�^�����
	for (int i = 0; i < BUFFER_NUM; i++) {
		free(whdr[i].lpData);
	}

	return -1;
}