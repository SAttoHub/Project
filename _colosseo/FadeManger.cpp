//#include "FadeManger.h"
//#include "Input.h"
//
//void FadeManger::CreateFadeRender()
//{
//	m_Buffer = std::make_unique<FadeBuffer>();
//	m_Buffer->Initialize();
//	m_Buffer2 = std::make_unique<FadeBuffer>();
//	m_Buffer2->InitializeFadeShader();
//
//	// �O�V�[���摜�ۑ��p�����_�[����
//	CreateRenderTarget(BufferRenderName, DXGI_FORMAT_R8G8B8A8_UNORM, false);
//	// �t�F�[�h��V�[���摜�ۑ��p�����_�[����
//	CreateRenderTarget("AfterFade", DXGI_FORMAT_R8G8B8A8_UNORM, false);
//	// �t�F�[�h�V�[���p�����_�[
//	CreateRenderTarget("FadeScene", DXGI_FORMAT_R8G8B8A8_UNORM, false);
//}
//
//void FadeManger::SaveScreen()
//{
//	m_SaveFlag = true;
//	//GetRenderTexture(_RenderName);
//}
//
//void FadeManger::Save(int RenderNum)
//{
//	PreDraw(BufferRenderName, true);
//	m_Buffer->Draw(RenderNum);
//	PostDraw(BufferRenderName);
//	m_SaveFlag = false;
//}
//
//void FadeManger::FadeDraw(int RenderNum)
//{
//	PreDraw("AfterFade", true);
//	m_Buffer->Draw(RenderNum);
//	PostDraw("AfterFade");
//
//
//	PreDraw("FadeScene", true);
//	m_Buffer->DrawFade(GetRenderTexture(BufferRenderName), GetRenderTexture("AfterFade"));
//	PostDraw("FadeScene");
//}
//
//void FadeManger::Test()
//{
//	if (Input::isKeyTrigger(DIK_P)) {
//		SaveScreen();
//	}
//
//	if (Input::isKey(DIK_O)) {
//		isFade = true;
//	}
//	else {
//		isFade = false;
//	}
//}
