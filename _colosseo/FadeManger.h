//#pragma once
//#include "RenderManager.h"
//#include "Singleton.h"
//#include "FadeBuffer.h"
//
//class FadeManger : public Singleton<FadeManger> {
//	friend class Singleton<FadeManger>;
//
//	std::string BufferRenderName = "Buffer";
//
//	std::unique_ptr<FadeBuffer> m_Buffer = nullptr;
//	std::unique_ptr<FadeBuffer> m_Buffer2 = nullptr;
//public:
//	void CreateFadeRender();
//
//	bool m_SaveFlag = false;
//	// �P�t���[���O�̕`�挋�ʂ�ۑ�����(�����ł͂Ȃ����݃t���[���̍Ō��)
//	void SaveScreen();
//
//	void Save(int RenderNum);
//
//	bool isFade = false;
//	void FadeDraw(int RenderNum);
//
//	void Test();
//};