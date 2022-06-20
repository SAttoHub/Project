#pragma once
#include "DirectXBase.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>


class XAudio2VoiceCallback : public IXAudio2VoiceCallback {
public:
    //�{�C�X�����p�X�����J�n��
    STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};

    //�{�C�X�����p�X�̏I����
    STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};

    //�o�b�t�@�X�g���[���̍Đ����I��������
    STDMETHOD_(void, OnStreamEnd) (THIS) {};

    //�o�b�t�@�̎g�p�J�n��
    STDMETHOD_(void, OnBufferStart) (THIS_ void *pBufferContext) {};

    //�o�b�t�@�̖����ɒB�����Ƃ�
    STDMETHOD_(void, OnBufferEnd) (THIS_ void *pBufferContext) {
        //�o�b�t�@���������
        delete[] pBufferContext;
    };

    //�Đ����[�v�ʒu�ɒB�����Ƃ�
    STDMETHOD_(void, OnLoopEnd) (THIS_ void *pBufferContext) {};

    //�{�C�X�̎��s�G���[��
    STDMETHOD_(void, OnVoiceError) (THIS_ void *pBufferContext, HRESULT Error) {};
};

//�t�@�C���ǂݍ��ݗp�\����
//�`�����N�w�b�_
struct Chunk {
	char id[4]; //�`�����N����ID
	int32_t size; //�`�����N�T�C�Y
};
//RIFF�w�b�_�`�����N
struct RiffHeader {
	Chunk chunk; //"RIFF"
	char type[4]; //"WAVE"
};
//FMT�`�����N
struct FormatChunk {
	Chunk chunk; //"fmt"
	WAVEFORMAT fmt; //�g�`�t�H�[�}�b�g
};


struct SoundData {
    const char *FileName;
    IXAudio2SourceVoice *pSourceVoice = nullptr;
    Chunk data;
    char *pBuffer;

    void DeleteData() {
        if (pSourceVoice != nullptr) {
            pSourceVoice->DestroyVoice();
            delete[] pBuffer;
        }
    };
};

class DXAudio {
	static HRESULT result;
    static ComPtr<IXAudio2> xAudio2;
    static IXAudio2MasteringVoice *masterVoice;

    static XAudio2VoiceCallback voiceCallback;
public:
    static void XAudioInit(); //�������ƃ}�X�^�Đ�
    static SoundData LoadWave(const char *filename);
    
    

    static void Play(SoundData *Data, float volume, bool Loop);
    static void Stop(SoundData *Data);
    static void SeekStart(SoundData *Data);

    static void PlayWave(const char *filename, float volume, bool Loop);
};