#include "Game.h"
#include "imguiUse.h"
#include "RenderManager.h"

void Game::Initialize()
{
	Framework::Initialize();

	game = new GameManager(&DX, &Tex, win, Pipeline, Cam);
	game->Init();
	//�}�E�X�J�[�\����\�����Ȃ�
	CursorShow(true);

	//��ʊE�[�x
	InterpSize = 380.0f;
	Focus = 30.0f;
	FocusSize = 20.0f;
	UseFlag = true;
	GaussianEffectX = new Gaussian;
	GaussianEffectX->Initialize(0);
	GaussianEffectY = new Gaussian;
	GaussianEffectY->Initialize(1);
	GaussianEffectX2 = new Gaussian;
	GaussianEffectX2->Initialize(0);
	GaussianEffectY2 = new Gaussian;
	GaussianEffectY2->Initialize(1);
	GaussianEffectX_b = new Gaussian;
	GaussianEffectX_b->Initialize(0);
	GaussianEffectY_b = new Gaussian;
	GaussianEffectY_b->Initialize(1);
	DOF = new DepthOfField;
	DOF->Initialize(InterpSize, Focus, FocusSize, UseFlag);

	//�u���[��
	bloom = new Bloom;
	bloom->Initialize();
	

	BloomFlag = true;
	DOFFlag = false;
	//�V���h�E
	ShadowMapUse = true;
	Shadow_Map_Light = new ShadowMapLight;
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f,250.0f, -175.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
	shadowMapping = new ShadowMapping;
	shadowMapping->Initialize(ShadowMapUse);

	//�r�l�b�g
	VignetteInfluence = 0.2f;
	UseVignette = true;
	vignette = new Vignette;
	vignette->Initialize(VignetteInfluence, UseVignette);

	//�A�E�g���C��
	/*UseOutLine = true;
	outLine = new OutLine;
	outLine->Initialize(UseOutLine);*/

	CreateRenderTarget("DOF_Depth", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Camera_Depth", DXGI_FORMAT_R32_FLOAT, true);
	CreateRenderTarget("Light_Depth", DXGI_FORMAT_R32_FLOAT, true, DirectX::XMINT2(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2));
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
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f, 250.0f, -175.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));

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
	//shadowMapping->Draw(GetRenderTexture("BaseGameScene"), GetRenderTexture("Light_Depth"), GetRenderTexture("Camera_Depth"));
	PostDraw("ShadowMap_Shadow");

#pragma region Bloom
	// Bloom ����F�݂̂̕`��
	PreDraw("Bloom_Base_Color");
	game->BloomDraw();
	PostDraw("Bloom_Base_Color");
	// Bloom �F�e�N�X�`���� X �����Ƀu���[��������
	PreDraw("Bloom_Gauss_X");
	GaussianEffectX_b->Draw(GetRenderTexture("Bloom_Base_Color"), 0.005f);
	PostDraw("Bloom_Gauss_X");
	// Bloom �F�e�N�X�`���� Y �����Ƀu���[��������
	PreDraw("Bloom_Gauss_Y");
	GaussianEffectY_b->Draw(GetRenderTexture("Bloom_Gauss_X"), 0.005f);
	PostDraw("Bloom_Gauss_Y");
	// �ŏI���ʂ�`��
	PreDraw("Bloom_Result");
	bloom->Draw(GetRenderTexture("ShadowMap_Shadow"), GetRenderTexture("Bloom_Gauss_Y"), GetRenderTexture("Camera_Depth"), GetRenderTexture("Bloom_Depth"));
	PostDraw("Bloom_Result");
#pragma endregion

#pragma region �A�E�g���C��
	/*PreDraw("OutLine");
	outLine->Draw(GetRenderTexture("Bloom_Result"), GetDepthTexture("Camera_Depth"));
	PostDraw("OutLine");*/
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
	GaussianEffectX2->Draw(GetRenderTexture("Bloom_Result"), 0.1f);
	PostDraw("DOF_Gauss_X2");
	// ��ŕ`�悵�����̂� Y �����ɃK�E�X��������Q
	PreDraw("DOF_Gauss_Y2");
	GaussianEffectY2->Draw(GetRenderTexture("DOF_Gauss_X2"), 0.1f);
	PostDraw("DOF_Gauss_Y2");
	// ��ʊE�[�x��K������
	PreDraw("DOF_Result");
	DOF->Draw(GetRenderTexture("Bloom_Result"), GetRenderTexture("DOF_Gauss_Y2"), GetRenderTexture("DOF_Gauss_Y"), GetRenderTexture("DOF_Depth"));
	PostDraw("DOF_Result");
#pragma endregion

	DXBase.ClearDepthBuffer();
	vignette->Draw(GetRenderTexture("DOF_Result"));

#pragma region ImGui
	/*DirectX::XMFLOAT2 p1 = Camera::ConvertWorldToScreen(XMFLOAT3(game->game.saku4->position.x, game->game.saku4->position.y, game->game.saku4->position.z));
	ImGui::SetNextWindowPos(ImVec2(p1.x - 200, p1.y - 160), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("saku1");
	ImGui::Checkbox("Bloom", &game->game.bloomFlag1);
	ImGui::End();
	DirectX::XMFLOAT2 p2 = Camera::ConvertWorldToScreen(XMFLOAT3(game->game.saku5->position.x, game->game.saku5->position.y, game->game.saku5->position.z));
	ImGui::SetNextWindowPos(ImVec2(p2.x - 100, p2.y - 130), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("saku2");
	ImGui::Checkbox("Bloom", &game->game.bloomFlag2);
	ImGui::End();
	*/

	float AddY = -200;

	/*ImGui::SetNextWindowPos(ImVec2(1000, 260 + AddY), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 200 + AddY), 1);
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
	
	ImGui::SetNextWindowPos(ImVec2(1000, 470 + AddY), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60 + AddY), 1);
	ImGui::Begin("ShadowMap");
	ImGui::Checkbox("isUse", &ShadowMapUse);
	ImGui::End();
	*/
	/*ImGui::SetNextWindowPos(ImVec2(1000, 540 + AddY), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 120 + AddY), 1);
	ImGui::Begin("Vignette");
	ImGui::SliderFloat("Influence", &VignetteInfluence, 0.0f, 10.0f);
	ImGui::Checkbox("isUse", &UseVignette);
	if (ImGui::Button("Reset")) {
		VignetteInfluence = 0.2f;
		UseVignette = true;
	}
	ImGui::End();*/
	/*ImGui::SetNextWindowPos(ImVec2(1000, 0), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 400), 1);
	ImGui::Begin("Camera");
	ImGui::Text("CamX : %f", Camera::eye.x);
	ImGui::Text("CamY : %f", Camera::eye.y);
	ImGui::Text("CamZ : %f", Camera::eye.z);
	ImGui::Text("targetX : %f", Camera::target.x);
	ImGui::Text("targetY : %f", Camera::target.y);
	ImGui::Text("targetZ : %f", Camera::target.z);
	ImGui::End();*/
	
	imguiUse::Instance()->CommandExcute(true);
#pragma endregion

	shadowMapping->SetUse(ShadowMapUse);
	vignette->SetState(VignetteInfluence, UseVignette);
	DOF->SetState(InterpSize, Focus, FocusSize, UseFlag);

	// 2D�v���~�e�B�u�`��
	Primitive2D::Instance()->Draw();

    //-----------------�����܂Ńv���O�����L��-----------------//

   //-----------------���߂̎��s�ƃo�b�t�@�̃t���b�v-----------------//
	DXBase.DoScreen();

    //FPS
      fps->FPS_Update();

	if (Input::isKeyTrigger(DIK_O)) {
		int i = 0;
	}
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
