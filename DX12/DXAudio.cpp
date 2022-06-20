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
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	if (file.fail()) {
		exit(1);
	}

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		exit(1);
	}
	//fmtチャンクまで飛ばす
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
	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char *)&format, sizeof(format));
	//dataチャンクまで飛ばす
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
	//Dataチャンクの読み込み
	Chunk data;
	file.read((char *)&data, sizeof(data));
	//Dataチャンクのデータ部（波形データ）の読み込み
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	//波形フォーマットの生成
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//波形フォーマットを元にSourceVoiceの生成
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
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	if (file.fail()) {
		exit(1);
	}

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		exit(1);
	}

	//fmtチャンクまで飛ばす
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

	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char *)&format, sizeof(format));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		exit(1);
	}

	//dataチャンクまで飛ばす
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

	//Dataチャンクの読み込み
	Chunk data;
	file.read((char *)&data, sizeof(data));

	//Dataチャンクのデータ部（波形データ）の読み込み
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	//波形フォーマットの生成
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice *pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if (FAILED(result)) {
		delete[] pBuffer;
		return;
	}
	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE *)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	//音量の変更
	result = pSourceVoice->SetVolume(volume);
	//波形データの再生
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
		//再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = (BYTE *)Data->pBuffer;
		buf.pContext = Data->pBuffer;
		buf.Flags = XAUDIO2_END_OF_STREAM;
		buf.AudioBytes = Data->data.size;
		if (Loop) {
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		//音量の変更
		result = Data->pSourceVoice->SetVolume(volume);
		//波形データの再生
		result = Data->pSourceVoice->SubmitSourceBuffer(&buf);
		result = Data->pSourceVoice->Start();
	}
}
