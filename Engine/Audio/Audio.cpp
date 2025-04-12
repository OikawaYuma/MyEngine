#include "Audio.h"
#include <cassert>

Audio::ComPtr<IXAudio2> Audio::xAudio2_;

IXAudio2SourceVoice* Audio::pSourceVoice[soundDataMaxSize];

SoundData Audio::soundData_[soundDataMaxSize];

uint32_t Audio::soundHandle = 0u;

Audio* Audio::GetInstance()
{
	static Audio instance;
	return &instance;
}

void Audio::Initialize()
{
	// XAudioエンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);


}

void Audio::Release()
{
	xAudio2_.Reset();
}

uint32_t Audio::SoundLoadWave(const char* filename)
{
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダ―の確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	
	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();
	
	// returnするための音声データ
	soundData_[soundHandle];

	soundData_[soundHandle].wfek = format.fmt;
	soundData_[soundHandle].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData_[soundHandle].bufferSize = data.size;
	uint32_t returnSoundHandle = soundHandle;
	soundHandle++;

	return returnSoundHandle;
}

void Audio::SoundUnload(uint32_t audioHandle) {

	// バッファのメモリを解放
	delete[] soundData_[audioHandle].pBuffer;

	soundData_[audioHandle].pBuffer = 0;
	soundData_[audioHandle].bufferSize = 0;
	soundData_[audioHandle].wfek = {};
}

void Audio::SoundPlayWave(IXAudio2* xAudio2,uint32_t audioHandle,bool loopFlag)
{
	HRESULT result;

	// 波形フォーマットを元にSorceVoiceの生成
	pSourceVoice[audioHandle] = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice[audioHandle], &soundData_[audioHandle].wfek);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[audioHandle].pBuffer;
	buf.AudioBytes = soundData_[audioHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) {
		// 無限ループ
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	result = pSourceVoice[audioHandle]->SubmitSourceBuffer(&buf);
	result = pSourceVoice[audioHandle]->SetVolume(0.5f);
	result = pSourceVoice[audioHandle]->Start();
}

void Audio::SoundStopWave(uint32_t audioHandle)
{
	HRESULT result;
	

	// 波形データの再生
	//result = pSourceVoice->SubmitSourceBuffer(&buf);
	// 波形フォーマットを元にSorceVoiceの生成
	result = pSourceVoice[audioHandle]->Stop();
	

}

void Audio::SoundLoopWave(IXAudio2* xAudio2, const SoundData& soundData)
{
	xAudio2;
	soundData;
	//HRESULT result;


	// 波形データの再生
	//result = pSourceVoice->SubmitSourceBuffer(&buf);
	// 波形フォーマットを元にSorceVoiceの生成
	


}
