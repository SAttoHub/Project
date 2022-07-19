#include "Game.h"
#include "ExtraTypeFunc.h"
#include "imguiUse.h"
#include "RenderManager.h"

void Game::Initialize()
{
	Framework::Initialize();

	RDot = Tex.LoadTexture("Resource/image/redDot.png");
	CenterDot = SP.ExtendSpriteCreate(WINDOW_WIDTH / 2 - 4, WINDOW_HEIGHT / 2 - 4, WINDOW_WIDTH / 2 + 4, WINDOW_HEIGHT / 2 + 4, RDot, SpriteNormal);

	game = new GameManager(&DX, &SP, &Tex, win, Pipeline, Cam);
	game->Init();
	//�}�E�X�J�[�\����\�����Ȃ�
	CursorShow(true);

	//��ʊE�[�x
	InterpSize = 20.0f;
	Focus = 30.0f;
	FocusSize = 20.0f;
	UseFlag = true;
	GaussianEffectX = new Gaussian;
	GaussianEffectX->Initialize(0);
	GaussianEffectY = new Gaussian;
	GaussianEffectY->Initialize(1);
	DOF = new DepthOfField;
	DOF->Initialize(InterpSize, Focus, FocusSize, UseFlag);

	//�u���[��
	bloom = new Bloom;
	bloom->Initialize();
	

	BloomFlag = true;
	DOFFlag = true;
	//�V���h�E
	ShadowMapUse = true;
	Shadow_Map_Light = new ShadowMapLight;
	ShadowMapLight::SetLightPos(XMFLOAT3(500.0f,500.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
	shadowMapping = new ShadowMapping;
	shadowMapping->Initialize(ShadowMapUse);

	//�r�l�b�g
	VignetteInfluence = 0.2f;
	UseVignette = true;
	vignette = new Vignette;
	vignette->Initialize(VignetteInfluence, UseVignette);

	CreateRenderTarget("DOF_Depth", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Camera_Depth", DXGI_FORMAT_R32_FLOAT, true);
	CreateRenderTarget("Light_Depth", DXGI_FORMAT_R32_FLOAT, true);
	CreateRenderTarget("BaseGameScene", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("ShadowMap_Shadow", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Bloom_Base_Color", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Bloom_Depth", DXGI_FORMAT_R32_FLOAT, true);
	CreateRenderTarget("Bloom_Gauss_X", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("Bloom_Gauss_Y", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("Bloom_Result", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("DOF_Gauss_X", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("DOF_Gauss_Y", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("DOF_Gauss_X2", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("DOF_Gauss_Y2", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("DOF_Result", DXGI_FORMAT_R8G8B8A8_UNORM, false);
}

void Game::Update()
{
    //-----------------��ʂ̃N���A-----------------//
    DXBase.ClearScreen();
	imguiUse *im = imguiUse::Instance();
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    //-----------------�L�[�{�[�h�̓��͏�Ԃ̍X�V-----------------//
    Input::Update();
    // WinAPIInput::Update();
    Primitive2D::Instance()->Update();
    Primitive3D::Instance()->Update();

    //-----------------��������v���O�����L��-----------------//
    game->Update();
    DX.AllObjectUpdate();

	Primitive2D::Instance()->BackDraw();
	ShadowMapLight::SetLightPos(XMFLOAT3(500.0f, 500.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));

#pragma region �e�[�x�l�ۑ��p�̕`��
	//DOF�p�[�x
	PreDraw("DOF_Depth");
	game->DOFDepthDraw();
	PostDraw("DOF_Depth");
	//�J�������_�̐[�x�`��
	PreDraw("Camera_Depth");
	game->DepthDraw();
	PostDraw("Camera_Depth");
	//���C�g���_�̐[�x�`��
	PreDraw("Light_Depth");
	game->ShadowDraw();
	PostDraw("Light_Depth");
	// Bloom �������Ɏg�p����[�x�l�̕ۑ�
	PreDraw("Bloom_Depth");
	game->BloomDepthDraw();
	PostDraw("Bloom_Depth");
#pragma endregion

	//�|�X�g�G�t�F�N�g�����̃Q�[�����
	PreDraw("BaseGameScene");
	game->Draw();
	PostDraw("BaseGameScene");

	//�V���h�E�}�b�v
	PreDraw("ShadowMap_Shadow");
	shadowMapping->Draw(GetRenderTexture("BaseGameScene"), GetRenderTexture("Light_Depth"), GetDepthTexture("Camera_Depth"));
	PostDraw("ShadowMap_Shadow");

#pragma region Bloom
	// Bloom ����F�݂̂̕`��
	PreDraw("Bloom_Base_Color");
	game->BloomDraw();
	PostDraw("Bloom_Base_Color");
	// Bloom �F�e�N�X�`���� X �����Ƀu���[��������
	PreDraw("Bloom_Gauss_X");
	GaussianEffectX->Draw(GetRenderTexture("Bloom_Base_Color"), 0.005f);
	PostDraw("Bloom_Gauss_X");
	// Bloom �F�e�N�X�`���� Y �����Ƀu���[��������
	PreDraw("Bloom_Gauss_Y");
	GaussianEffectY->Draw(GetRenderTexture("Bloom_Gauss_X"), 0.005f);
	PostDraw("Bloom_Gauss_Y");
	// �ŏI���ʂ�`��
	PreDraw("Bloom_Result");
	bloom->Draw(GetRenderTexture("ShadowMap_Shadow"), GetRenderTexture("Bloom_Gauss_Y"), GetRenderTexture("Camera_Depth"), GetRenderTexture("Bloom_Depth"));
	PostDraw("Bloom_Result");
#pragma endregion
#pragma region ��ʊE�[�x
	// ��ʊE�[�x��K������O�܂ł̃|�X�g�G�t�F�N�g��K�������̍ŏI�`�挋�ʂ� X �����ɃK�E�X��������P
	PreDraw("DOF_Gauss_X");
	GaussianEffectX->Draw(GetRenderTexture("Bloom_Result"), 0.03f);
	PostDraw("DOF_Gauss_X");
	// ��ŕ`�悵�����̂� Y �����ɃK�E�X��������P
	PreDraw("DOF_Gauss_Y");
	GaussianEffectY->Draw(GetRenderTexture("DOF_Gauss_X"), 0.03f);
	PostDraw("DOF_Gauss_Y");
	// ��ʊE�[�x��K������O�܂ł̃|�X�g�G�t�F�N�g��K�������̍ŏI�`�挋�ʂ� X �����ɃK�E�X��������Q
	PreDraw("DOF_Gauss_X2");
	GaussianEffectX->Draw(GetRenderTexture("Bloom_Result"), 0.1f);
	PostDraw("DOF_Gauss_X2");
	// ��ŕ`�悵�����̂� Y �����ɃK�E�X��������Q
	PreDraw("DOF_Gauss_Y2");
	GaussianEffectY->Draw(GetRenderTexture("DOF_Gauss_X2"), 0.1f);
	PostDraw("DOF_Gauss_Y2");
	// ��ʊE�[�x��K������
	PreDraw("DOF_Result");
	DOF->Draw(GetRenderTexture("Bloom_Result"), GetRenderTexture("DOF_Gauss_Y2"), GetRenderTexture("DOF_Gauss_Y"), GetRenderTexture("DOF_Depth"));
	PostDraw("DOF_Result");
#pragma endregion

	DXBase.ClearDepthBuffer();
	vignette->Draw(GetRenderTexture("DOF_Result"));

#pragma region ImGui
	ImGui::SetNextWindowPos(ImVec2(1000, 260), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 200), 1);
	ImGui::Begin("DOF");
	ImGui::SliderFloat("InterpSize", &InterpSize, 1.0f, 1000.0f);
	ImGui::SliderFloat("Focus", &Focus, 1.0f, 1000.0f);
	ImGui::SliderFloat("FocusSize", &FocusSize, 1.0f, 1000.0f);
	ImGui::Checkbox("isUse", &UseFlag);
	if (ImGui::Button("Reset")) {
		InterpSize = 20.0f;
		Focus = 30.0f;
		FocusSize = 20.0f;
		UseFlag = true;
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(1000, 470), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("ShadowMap");
	ImGui::Checkbox("isUse", &ShadowMapUse);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(1000, 540), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 120), 1);
	ImGui::Begin("Vignette");
	ImGui::SliderFloat("Influence", &VignetteInfluence, 0.0f, 10.0f);
	ImGui::Checkbox("isUse", &UseVignette);
	if (ImGui::Button("Reset")) {
		VignetteInfluence = 0.2f;
		UseVignette = true;
	}
	ImGui::End();

	imguiUse::Instance()->CommandExcute(true);
#pragma endregion

	shadowMapping->SetUse(ShadowMapUse);
	vignette->SetState(VignetteInfluence, UseVignette);
	DOF->SetState(InterpSize, Focus, FocusSize, UseFlag);

	//�X�v���C�g�֘A�`��
	Primitive2D::Instance()->Draw();

    //-----------------�����܂Ńv���O�����L��-----------------//

    //-----------------���߂̎��s�ƃo�b�t�@�̃t���b�v-----------------//
    DXBase.DoScreen();
    //FPS
    fps->FPS_Update();
}

void Game::Finalize()
{
	delete GaussianEffectX;
	delete GaussianEffectY;
	delete DOF;
	delete shadowMapping;
	delete bloom;
	Framework::Finalize();
}

void Game::Run(const int iCmdShow)
{
    Initialize();
	ShowWindow(GetWindow()->WinHandle, iCmdShow);
	MSG msg{}; //���b�Z�[�W���[�v�p
	while (1) {
		// ���b�Z�[�W���[�v
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); //�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg); //�v���[�W���Ƀ��b�Z�[�W�𑗂�
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		Update();
		//Escape�L�[�������ꂽ�Ƃ��Ƀ��[�v�𔲂���
		if (Input::isKeyTrigger(DIK_ESCAPE)) {
			break;
		}
	}
    Finalize();
}
