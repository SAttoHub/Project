#include "Game.h"
#include "ExtraTypeFunc.h"
#include "imguiUse.h"

void Game::Initialize()
{
	Framework::Initialize();

	RDot = Tex.LoadTexture("Resource/image/redDot.png");
	CenterDot = SP.ExtendSpriteCreate(WINDOW_WIDTH / 2 - 4, WINDOW_HEIGHT / 2 - 4, WINDOW_WIDTH / 2 + 4, WINDOW_HEIGHT / 2 + 4, RDot, SpriteNormal);
	//int skydomeDataNum = LoadModelOBJ("skydome", "skydome");
	//skydome = DX.CreateObject(GetModelData(skydomeDataNum), XMFLOAT3{ Cam->target.x, Cam->target.y, Cam->target.z }, ObjectNormal);
	//DX.object3ds[DX.object3ds.size() - 1].scale = { 5,5,5 };

	game = new GameManager(&DX, &SP, &Tex, win, Pipeline, Cam);
	game->Init();
	//�}�E�X�J�[�\����\�����Ȃ�
	CursorShow(true);

	postEffect = new PostEffect;
	postEffect->Initialize(DXGI_FORMAT_R8G8B8A8_UNORM);
	postEffect2 = new PostEffect;
	postEffect2->Initialize(DXGI_FORMAT_R8G8B8A8_UNORM);

	//��ʊE�[�x
	InterpSize = 20.0f;
	Focus = 30.0f;
	FocusSize = 20.0f;
	UseFlag = true;
	GaussianEffectX = new Gaussian;
	GaussianEffectX->Initialize(0, 0.03f);
	GaussianEffectY = new Gaussian;
	GaussianEffectY->Initialize(1, 0.03f);
	GaussianEffectX2 = new Gaussian;
	GaussianEffectX2->Initialize(0, 0.1f);
	GaussianEffectY2 = new Gaussian;
	GaussianEffectY2->Initialize(1, 0.1f);
	DOF = new DepthOfField;
	DOF->Initialize(InterpSize, Focus, FocusSize, UseFlag);
	depth = new Depth;
	depth->Initialize();

	//�u���[��
	bloom = new Bloom;
	bloom->Initialize();
	GaussianEffectXBloom = new Gaussian;
	GaussianEffectXBloom->Initialize(0, 0.005f);
	GaussianEffectYBloom = new Gaussian;
	GaussianEffectYBloom->Initialize(1, 0.005f);
	depth3 = new Depth2;
	depth3->Initialize();

	BloomFlag = true;
	DOFFlag = true;
	//�V���h�E
	ShadowMapUse = true;
	depth2 = new Depth2;
	depth2->Initialize();
	Shadow_Map_Light = new ShadowMapLight;
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f,500.0f, 0.01f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
	shadowMapping = new ShadowMapping;
	shadowMapping->Initialize(ShadowMapUse);

	//�r�l�b�g
	VignetteInfluence = 0.2f;
	UseVignette = true;
	vignette = new Vignette;
	vignette->Initialize(VignetteInfluence, UseVignette);
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
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f, 500.0f, 0.01f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
	
	//postEffect->PreDrawScene(1);
 //   //Drawobject3d(skydome);
 //   game->Draw();

 //   //�f�o�b�O�e�L�X�g��Draw
 //   DXBase.ClearDepthBuffer();

	//postEffect->PostDrawScene();


	//GaussianEffectX->PreDrawScene(1);
	//postEffect->Draw(DirectXBase::rtvHeaps.Get());

	//DXBase.ClearDepthBuffer();
	//GaussianEffectX->PostDrawScene();
	//GaussianEffectX->Draw(postEffect->descHeapSRV.Get());
	
	
	//�[�x�l�ۑ��`��
	depth->PreDrawScene(1);
	game->DOFDepthDraw();
	depth->PostDrawScene();
	//�J�������_�̐[�x�`��
	depth2->PreDrawScene(1);
	game->DepthDraw();
	depth2->PostDrawScene();
	//���C�g���_�̐[�x�`��
	shadowMapping->PreDrawScene();
	game->ShadowDraw();
	shadowMapping->PostDrawScene();

	postEffect->PreDrawScene(1);
	game->Draw();
	postEffect->PostDrawScene();

	postEffect2->PreDrawScene(1);
	shadowMapping->Draw(postEffect->TexNum, shadowMapping->TexNum, depth2->TexNum2);
	postEffect2->PostDrawScene();


	//�u���[���p
	bloom->PreDrawScene();
	game->BloomDraw();
	bloom->PostDrawScene();
	depth3->PreDrawScene(1);
	game->BloomDepthDraw();
	depth3->PostDrawScene();
	GaussianEffectXBloom->PreDrawScene(1);
	GaussianEffectXBloom->Draw(bloom->TexNum);
	GaussianEffectXBloom->PostDrawScene();
	GaussianEffectYBloom->PreDrawScene(1);
	GaussianEffectYBloom->Draw(GaussianEffectXBloom->TexNum);
	GaussianEffectYBloom->PostDrawScene();

	bloom->PreDrawScene();
	bloom->Draw(postEffect2->TexNum, GaussianEffectYBloom->TexNum, depth2->TexNum, depth3->TexNum);
	bloom->PostDrawScene();


	GaussianEffectX->PreDrawScene(1);
	GaussianEffectX->Draw(bloom->TexNum);
	GaussianEffectX->PostDrawScene();

	GaussianEffectY->PreDrawScene(1);
	GaussianEffectY->Draw(GaussianEffectX->TexNum);
	GaussianEffectY->PostDrawScene();

	GaussianEffectX2->PreDrawScene(1);
	GaussianEffectX2->Draw(bloom->TexNum);
	GaussianEffectX2->PostDrawScene();

	GaussianEffectY2->PreDrawScene(1);
	GaussianEffectY2->Draw(GaussianEffectX2->TexNum);
	GaussianEffectY2->PostDrawScene();


	DOF->PreDrawScene();
	DOF->Draw(bloom->TexNum, GaussianEffectY2->TexNum, GaussianEffectY->TexNum, depth->TexNum);
	DOF->PostDrawScene();

	DXBase.ClearDepthBuffer();
	vignette->Draw(DOF->TexNum);


	ImGui::SetNextWindowPos(ImVec2(1000, 20), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("Buri_1");
	ImGui::Checkbox("Bloom", &game->game.BloomFlag1);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(1000, 90), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("Buri_2");
	ImGui::Checkbox("Bloom", &game->game.BloomFlag2);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(1000, 160), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 60), 1);
	ImGui::Begin("Buri_3");
	ImGui::Checkbox("Bloom", &game->game.BloomFlag3);
	ImGui::End();

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

	shadowMapping->SetUse(ShadowMapUse);
	vignette->SetState(VignetteInfluence, UseVignette);
	DOF->SetState(InterpSize, Focus, FocusSize, UseFlag);
	/*postEffect->PreDrawScene(1);
	game->Draw();
	postEffect->PostDrawScene();

	DXBase.ClearDepthBuffer();
	shadowMapping->Draw(postEffect->descHeapSRV.Get(), shadowMapping->descHeapSRV.Get(), depth2->descHeapSRV.Get());*/
	
	//DrawStrings::Instance()->DrawFormatString(XMFLOAT2(0, 0), 32, XMFLOAT4(0, 0, 0, 1), "E : �}�E�X�S������");
	//DrawStrings::Instance()->DrawFormatString(XMFLOAT2(0, 34), 32, XMFLOAT4(0, 0, 0, 1), "Q : �}�E�X�S���J�n");

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
	delete postEffect;
	delete GaussianEffectX;
	delete GaussianEffectY;
	delete DOF;
	delete depth;
	delete shadowMapping;
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
