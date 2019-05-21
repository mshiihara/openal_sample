#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

#include "WaveFile.h"


int main(void) {
	

	//
	// OpenAL�֘A�̕ϐ�
	//
	ALCcontext* context = nullptr;
	ALuint bufferId = 0;
	ALuint sourceId = 0;
	
	//
	// OpenAL��������
	//
	ALCdevice* device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	//
	// �o�b�t�@�ƃ\�[�X�𐶐�
	//
	alGenBuffers(1, &bufferId);
	alGenSources(1, &sourceId);

	//
	// Wave�t�@�C���̓ǂݍ���
	//
	WaveFile waveFile("magical30.wav");

	//
	// �X�e���I�ƃ��m�����𔻕�
	//
	ALenum format;
	// ���m����
	if (waveFile.getFormat()->nChannels == 1) {
		if (waveFile.getFormat()->wBitsPerSample == 8) { format = AL_FORMAT_MONO8; }
		else { format = AL_FORMAT_MONO16; }
	}
	// �X�e���I
	else {
		if (waveFile.getFormat()->wBitsPerSample == 8) { format = AL_FORMAT_STEREO8; }
		else { format = AL_FORMAT_STEREO16; }
	}
	
	//
	// �o�b�t�@�ɔg�`�f�[�^��]�����\�[�X�Ɛڑ�
	//
	byte* buffer = new byte[waveFile.getDataSize()];
	waveFile.read(buffer, waveFile.getDataSize());
	alBufferData(bufferId, format, buffer, waveFile.getDataSize(), waveFile.getFormat()->nSamplesPerSec);
	delete[] buffer;

	// �\�[�X�I�u�W�F�N�g�ɃI�[�f�B�I�o�b�t�@��ݒ�
	alSourcei(sourceId, AL_BUFFER, bufferId);

	//
	// �Đ�
	//
	alSourcePlay(sourceId);
	printf("�I������ɂ͉����L�[�������Ă�������\n");
	getchar();

	//
	// �Đ��ƒ�~���\�[�X�I�u�W�F�N�g�ƃI�[�f�B�I�o�b�t�@��j��
	//
	alSourceStop(bufferId);
	alDeleteSources(1, &bufferId);
	alDeleteBuffers(1, &bufferId);

	//
	// OpenAL��j��
	//
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}