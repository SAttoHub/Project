#include "Game.h"
#include "imguiUse.h"
#include "RenderManager.h"

void Game::Initialize()
{
	Framework::Initialize();

	game = new GameManager(&DX, &Tex, win, Pipeline, Cam);
	game->Init();
	
	//マウスカーソルを表示しない
	CursorShow(false);

	//被写界深度
	InterpSize = 380.0f;
	Focus = 30.0f;
	FocusSize = 20.0f;
	UseFlag = true;
	GaussianEffectX = std::make_unique<Gaussian>();
	GaussianEffectX->Initialize(0);
	GaussianEffectY = std::make_unique<Gaussian>();
	GaussianEffectY->Initialize(1);
	GaussianEffectX2 = std::make_unique<Gaussian>();
	GaussianEffectX2->Initialize(0);
	GaussianEffectY2 = std::make_unique<Gaussian>();
	GaussianEffectY2->Initialize(1);
	GaussianEffectX_b = std::make_unique<Gaussian>();
	GaussianEffectX_b->Initialize(0);
	GaussianEffectY_b = std::make_unique<Gaussian>();
	GaussianEffectY_b->Initialize(1);
	DOF = std::make_unique<DepthOfField>();
	DOF->Initialize(InterpSize, Focus, FocusSize, UseFlag);

	//ブルーム
	bloom = std::make_unique<Bloom>();
	bloom->Initialize();
	

	BloomFlag = true;
	DOFFlag = false;
	//シャドウ
	ShadowMapUse = true;
	Shadow_Map_Light = std::make_unique<ShadowMapLight>();
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f,350.0f, -245.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
	shadowMapping = std::make_unique<ShadowMapping>();
	shadowMapping->Initialize(ShadowMapUse);

	//ビネット
	VignetteInfluence = 0.2f;
	UseVignette = true;
	vignette = std::make_unique<Vignette>();
	vignette->Initialize(VignetteInfluence, UseVignette);

	//アウトライン
	/*UseOutLine = true;
	outLine = new OutLine;
	outLine->Initialize(UseOutLine);*/

	CreateRenderTarget("DOF_Depth", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Camera_Depth", DXGI_FORMAT_R32_FLOAT, true);
	CreateRenderTarget("Light_Depth", DXGI_FORMAT_R32_FLOAT, true, DirectX::XMINT2(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2));
	CreateRenderTarget("BaseGameScene", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("ShadowMap_Shadow", DXGI_FORMAT_R8G8B8A8_UNORM, true);
	CreateRenderTarget("Bloom_Base_Color", DXGI_FORMAT_R8G8B8A8_UNORM, true, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("Bloom_Depth", DXGI_FORMAT_R32_FLOAT, true, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("Bloom_Gauss_X", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("Bloom_Gauss_Y", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("Bloom_Result", DXGI_FORMAT_R8G8B8A8_UNORM, false);
	CreateRenderTarget("DOF_Gauss_X", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("DOF_Gauss_Y", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("DOF_Gauss_X2", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("DOF_Gauss_Y2", DXGI_FORMAT_R8G8B8A8_UNORM, false, DirectX::XMINT2(WINDOW_WIDTH, WINDOW_HEIGHT));
	CreateRenderTarget("DOF_Result", DXGI_FORMAT_R8G8B8A8_UNORM, false);

	PSf_Perf = PSf_Normal;
	PSf_Counter = -2;
}

void Game::Update()
{
    //-----------------画面のクリア-----------------//
    DXBase.ClearScreen();
	imguiUse *im = imguiUse::Instance();
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    //-----------------キーボードの入力状態の更新-----------------//
    Input::Update();
    // WinAPIInput::Update();
    Primitive2D::Instance()->Update();
    Primitive3D::Instance()->Update();

    //-----------------ここからプログラム記入-----------------//
	Camera::Update();
    game->Update();
    DX.AllObjectUpdate();

	Primitive2D::Instance()->BackDraw();
	ShadowMapLight::SetLightPos(XMFLOAT3(0.0f, 350.0f, -245.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));

#pragma region 各深度値保存用の描画
	if (DirectX3dObject::isUpdateOther) {
		//DOF用深度
		if (PSf_isDraw("DOF")) {
			PreDraw("DOF_Depth");
			game->DOFDepthDraw();
			PostDraw("DOF_Depth");
		}
		//カメラ視点の深度描画
		//if (PSf_isDraw("Shadow")) {
		PreDraw("Camera_Depth");
		game->DepthDraw();
		PostDraw("Camera_Depth");
			//ライト視点の深度描画
		if (PSf_isDraw("Shadow")) {
			PreDraw("Light_Depth");
			game->ShadowDraw();
			PostDraw("Light_Depth");
		}
		//}
		// Bloom 合成時に使用する深度値の保存
		if (PSf_isDraw("Bloom")) {
			PreDraw("Bloom_Depth");
			game->BloomDepthDraw();
			PostDraw("Bloom_Depth");
		}
	}
#pragma endregion

	//ポストエフェクト無しのゲーム画面
	PreDraw("BaseGameScene");
	game->Draw();
	Primitive3D::Instance()->Draw();
	PostDraw("BaseGameScene");

	
	//シャドウマップ
	PreDraw("ShadowMap_Shadow");
	shadowMapping->Draw(GetRenderTexture("BaseGameScene"), GetRenderTexture("Light_Depth"), GetDepthTexture("Camera_Depth"));
	PostDraw("ShadowMap_Shadow");
#pragma region Bloom
	if (DirectX3dObject::isUpdateOther) {
		// Bloom する色のみの描画
		if (PSf_isDraw("Bloom")) {
			PreDraw("Bloom_Base_Color");
			game->BloomDraw();
			PostDraw("Bloom_Base_Color");
			// Bloom 色テクスチャの X 方向にブラーをかける
			PreDraw("Bloom_Gauss_X", false);
			GaussianEffectX_b->Draw(GetRenderTexture("Bloom_Base_Color"), 0.005f);
			PostDraw("Bloom_Gauss_X");
			// Bloom 色テクスチャの Y 方向にブラーをかける
			PreDraw("Bloom_Gauss_Y", false);
			GaussianEffectY_b->Draw(GetRenderTexture("Bloom_Gauss_X"), 0.005f);
			PostDraw("Bloom_Gauss_Y");
		}
	}
	// 最終結果を描画
	PreDraw("Bloom_Result", false);
	bloom->Draw(GetRenderTexture("ShadowMap_Shadow"), GetRenderTexture("Bloom_Gauss_Y"), GetDepthTexture("BaseGameScene"), GetDepthTexture("Bloom_Depth"));
	PostDraw("Bloom_Result");

#pragma endregion

#pragma region アウトライン
		/*PreDraw("OutLine");
		outLine->Draw(GetRenderTexture("Bloom_Result"), GetDepthTexture("Camera_Depth"));
		PostDraw("OutLine");*/
#pragma endregion

#pragma region 被写界深度
	if (DirectX3dObject::isUpdateOther) {
		// 被写界深度を適応する前までのポストエフェクトを適応したの最終描画結果の X 方向にガウスをかける１
		if (PSf_isDraw("DOF")) {
			PreDraw("DOF_Gauss_X", false);
			GaussianEffectX->Draw(GetRenderTexture("Bloom_Result"), 0.03f);
			PostDraw("DOF_Gauss_X");
			// 上で描画したものの Y 方向にガウスをかける１
			PreDraw("DOF_Gauss_Y", false);
			GaussianEffectY->Draw(GetRenderTexture("DOF_Gauss_X"), 0.03f);
			PostDraw("DOF_Gauss_Y");
			// 被写界深度を適応する前までのポストエフェクトを適応したの最終描画結果の X 方向にガウスをかける２
			PreDraw("DOF_Gauss_X2", false);
			GaussianEffectX2->Draw(GetRenderTexture("Bloom_Result"), 0.1f);
			PostDraw("DOF_Gauss_X2");
			// 上で描画したものの Y 方向にガウスをかける２
			PreDraw("DOF_Gauss_Y2", false);
			GaussianEffectY2->Draw(GetRenderTexture("DOF_Gauss_X2"), 0.1f);
			PostDraw("DOF_Gauss_Y2");
		}
	}
		// 被写界深度を適応する
		PreDraw("DOF_Result", false);
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

	ImGui::SetNextWindowPos(ImVec2(1000, 260 + AddY), 1);
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
	
	ImGui::SetNextWindowPos(ImVec2(1000, 540 + AddY), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 120 + AddY), 1);
	ImGui::Begin("Vignette");
	ImGui::SliderFloat("Influence", &VignetteInfluence, 0.0f, 10.0f);
	ImGui::Checkbox("isUse", &UseVignette);
	if (ImGui::Button("Reset")) {
		VignetteInfluence = 0.2f;
		UseVignette = true;
	}
	ImGui::End();
	


	/*ImGui::SetNextWindowPos(ImVec2(1000, 540 + AddY), 1);
	ImGui::SetNextWindowSize(ImVec2(250, 120 + AddY), 1);
	ImGui::Begin("Config");
	ImGui::SliderFloat("fps :", &FPS::fps, 0.0f, 200.0f);
	ImGui::Checkbox("HighPerformance", &HighPerformance);
	ImGui::End();
	if (HighPerformance == false) {
		PSf_Perf = PSf_Normal;
	}
	else {
		PSf_Perf = PSf_High;
	}*/


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
	
	
#pragma endregion

	PSf_Counter++;
	if (PSf_Counter >= 2) {
		PSf_Counter = 0;
	}

	shadowMapping->SetUse(ShadowMapUse);
	vignette->SetState(VignetteInfluence, UseVignette);
	DOF->SetState(InterpSize, Focus, FocusSize, UseFlag);

	// 2Dプリミティブ描画
	Primitive2D::Instance()->Draw();
	imguiUse::Instance()->CommandExcute(true);

    //-----------------ここまでプログラム記入-----------------//

   //-----------------命令の実行とバッファのフリップ-----------------//
	DXBase.DoScreen();

    //FPS
    fps->FPS_Update();
}

void Game::Finalize()
{
	Framework::Finalize();
}

void Game::Run(const int iCmdShow)
{
    Initialize();
	ShowWindow(GetWindow()->WinHandle, iCmdShow);
	MSG msg{}; //メッセージループ用
	while (1) {
		// メッセージループ
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); //キー入力メッセージの処理
			DispatchMessage(&msg); //プロージャにメッセージを送る
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		Update();
		//Escapeキーが押されたときにループを抜ける
		if (Input::isKeyTrigger(DIK_ESCAPE)) {
			break;
		}
	}
    Finalize();
}

bool Game::PSf_isDraw(std::string RTName)
{
	if (PSf_Counter < 0) {
		return true;
	}
	switch (PSf_Perf)
	{
	case Game::PSf_High:
		return true;
		break;
	case Game::PSf_Normal:
		if (RTName == "Shadow" && (PSf_Counter == 1 || PSf_Counter == 0)) {
			return true;
		}
		else if (RTName == "Bloom" && (PSf_Counter == 1 || PSf_Counter == 0)) {
			return true;
		}
		else if (RTName == "DOF" && (PSf_Counter == 1)) {
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
