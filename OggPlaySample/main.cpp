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

	// �o�b�t�@
	ogg_int64_t  size = ov_pcm_total(&ovf, -1) * oggInfo->channels * 2;

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
	// �X�e���I�ƃ��m�����𔻕�
	//
	ALenum format;
	// ���m����
	if (oggInfo->channels == 1) {
		format = AL_FORMAT_MONO16;
	}
	// �X�e���I
	else {
		format = AL_FORMAT_STEREO16;
	}

	//
	// �o�b�t�@�ɔg�`�f�[�^��]�����\�[�X�Ɛڑ�
	//
	byte* buffer = new byte[size];
	int bitstream = -1;
	long readByte = 0;
	while (readByte < size) {
		readByte += ov_read(&ovf, (char*)(buffer + readByte), size, 0, 2, 1, &bitstream);
	}
	alBufferData(bufferId, format, buffer, size, oggInfo->rate);
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

	ov_clear(&ovf);

	getchar();
	return 0;
}