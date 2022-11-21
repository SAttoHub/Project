#include "DXAudio.h"
HRESULT DXAudio::result;
ComPtr<IXAudio2> DXAudio::xAudio2;
IXAudio2MasteringVoice *DXAudio::masterVoice;
XAudio2VoiceCallback DXAudio::voiceCallback;


void DXAudio::XAudioInit() {
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

SoundData DXAudio::LoadWave(const char *filename)
{
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	if (file.fail()) {
		exit(1);
	}

	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		exit(1);
	}
	//fmt�`�����N�܂Ŕ�΂�
	std::string id = "fmt ";
	while (true) {
		file.read((char *)id.data(), sizeof(char) * 4);
		if (id == "fmt ") {
			file.seekg(-4 * sizeof(char), std::ios_base::cur);
			break;
		}
		else {
			file.seekg(-3 * sizeof(char), std::ios_base::cur);
		}
	}
	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char *)&format, sizeof(format));
	//data�`�����N�܂Ŕ�΂�
	id = "data";
	while (true) {
		file.read((char *)id.data(), sizeof(char) * 4);
		if (id == "data") {
			file.seekg(-4 * sizeof(char), std::ios_base::cur);
			break;
		}
		else {
			file.seekg(-3 * sizeof(char), std::ios_base::cur);
		}
	}
	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char *)&data, sizeof(data));
	//Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//wave�t�@�C�������
	file.close();

	WAVEFORMATEX wfex{};
	//�g�`�t�H�[�}�b�g�̐���
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice *pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if (FAILED(result)) {
		delete[] pBuffer;
		exit(1);
	}

	SoundData Data;
	Data.FileName = filename;
	Data.data = data;
	Data.pBuffer = pBuffer;
	Data.pSourceVoice = pSourceVoice;

	return Data;
}



void DXAudio::PlayWave(const char *filename, float volume, bool Loop) {
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	if (file.fail()) {
		exit(1);
	}

	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		exit(1);
	}

	//fmt�`�����N�܂Ŕ�΂�
	std::string id = "fmt ";
	while (true) {
		file.read((char *)id.data(), sizeof(char) * 4);
		if (id == "fmt "){
			file.seekg(-4 * sizeof(char), std::ios_base::cur);
			break;
		}
		else {
			file.seekg(-3 * sizeof(char), std::ios_base::cur);
		}
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char *)&format, sizeof(format));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		exit(1);
	}

	//data�`�����N�܂Ŕ�΂�
	id = "data";
	while (true) {
		file.read((char *)id.data(), sizeof(char) * 4);
		if (id == "data") {
			file.seekg(-4 * sizeof(char), std::ios_base::cur);
			break;
		}
		else {
			file.seekg(-3 * sizeof(char), std::ios_base::cur);
		}
	}

	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char *)&data, sizeof(data));

	//Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//wave�t�@�C�������
	file.close();

	WAVEFORMATEX wfex{};
	//�g�`�t�H�[�}�b�g�̐���
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice *pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if (FAILED(result)) {
		delete[] pBuffer;
		return;
	}
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE *)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	//���ʂ̕ύX
	result = pSourceVoice->SetVolume(volume);
	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void DXAudio::Stop(SoundData *Data) {
	if (Data->pSourceVoice != nullptr) {
		//Data->pBuffer->
		Data->pSourceVoice->Stop();
		//Data->pSourceVoice->FlushSourceBuffers();
		//Data->pSourceVoice->
	}
}

void DXAudio::SeekStart(SoundData *Data) {
	Data->DeleteData();
	*Data = DXAudio::LoadWave(Data->FileName);
}

void DXAudio::Play(SoundData *Data, float volume, bool Loop) {
	if (Data->pSourceVoice != nullptr) {
		//Data->pBuffer = 0;
		//�Đ�����g�`�f�[�^�̐ݒ�
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = (BYTE *)Data->pBuffer;
		buf.pContext = Data->pBuffer;
		buf.Flags = XAUDIO2_END_OF_STREAM;
		buf.AudioBytes = Data->data.size;
		if (Loop) {
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		//���ʂ̕ύX
		result = Data->pSourceVoice->SetVolume(volume);
		//�g�`�f�[�^�̍Đ�
		result = Data->pSourceVoice->SubmitSourceBuffer(&buf);
		result = Data->pSourceVoice->Start();
	}
}
