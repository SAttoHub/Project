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
//	// １フレーム前の描画結果を保存する(即時ではなく現在フレームの最後に)
//	void SaveScreen();
//
//	void Save(int RenderNum);
//
//	bool isFade = false;
//	void FadeDraw(int RenderNum);
//
//	void Test();
//};