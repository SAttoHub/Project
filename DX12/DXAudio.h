#pragma once
#include "DirectXBase.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>


class XAudio2VoiceCallback : public IXAudio2VoiceCallback {
public:
    //ボイス処理パスおｎ開始時
    STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};

    //ボイス処理パスの終了時
    STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};

    //バッファストリームの再生が終了した時
    STDMETHOD_(void, OnStreamEnd) (THIS) {};

    //バッファの使用開始時
    STDMETHOD_(void, OnBufferStart) (THIS_ void *pBufferContext) {};

    //バッファの末尾に達したとき
    STDMETHOD_(void, OnBufferEnd) (THIS_ void *pBufferContext) {
        //バッファを解放する
        delete[] pBufferContext;
    };

    //再生ループ位置に達したとき
    STDMETHOD_(void, OnLoopEnd) (THIS_ void *pBufferContext) {};

    //ボイスの実行エラー時
    STDMETHOD_(void, OnVoiceError) (THIS_ void *pBufferContext, HRESULT Error) {};
};

//ファイル読み込み用構造体
//チャンクヘッダ
struct Chunk {
	char id[4]; //チャンク毎のID
	int32_t size; //チャンクサイズ
};
//RIFFヘッダチャンク
struct RiffHeader {
	Chunk chunk; //"RIFF"
	char type[4]; //"WAVE"
};
//FMTチャンク
struct FormatChunk {
	Chunk chunk; //"fmt"
	WAVEFORMAT fmt; //波形フォーマット
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
    static void XAudioInit(); //初期化とマスタ再生
    static SoundData LoadWave(const char *filename);
    
    

    static void Play(SoundData *Data, float volume, bool Loop);
    static void Stop(SoundData *Data);
    static void SeekStart(SoundData *Data);

    static void PlayWave(const char *filename, float volume, bool Loop);
};