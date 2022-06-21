#include "Game.h"
#include "ExtraTypeFunc.h"

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
	//マウスカーソルを表示しない
	CursorShow(true);

	postEffect = new PostEffect;
	postEffect->Initialize();

	GaussianEffect = new Gaussian;
	GaussianEffect->Initialize();
}

void Game::Update()
{
    //-----------------画面のクリア-----------------//
    DXBase.ClearScreen();

    //-----------------キーボードの入力状態の更新-----------------//
    Input::Update();
    // WinAPIInput::Update();
    Primitive2D::Instance()->Update();
    Primitive3D::Instance()->Update();

    //-----------------ここからプログラム記入-----------------//
    game->Update();
    DX.AllObjectUpdate();

	Primitive2D::Instance()->BackDraw();

	postEffect->PreDrawScene();
    //Drawobject3d(skydome);
    game->Draw();

    //デバッグテキストのDraw
    DXBase.ClearDepthBuffer();

	postEffect->PostDrawScene();

	//GaussianEffect->PreDrawScene();

	postEffect->Draw();

	
	//GaussianEffect->PostDrawScene();
	//GaussianEffect->Draw(postEffect->descHeapSRV.Get());
	

	//スプライト関連描画
	Primitive2D::Instance()->Draw();
	//SP.SpriteDraw(&CenterDot);

    //-----------------ここまでプログラム記入-----------------//

    //-----------------命令の実行とバッファのフリップ-----------------//
    DXBase.DoScreen();
    //FPS
    fps->FPS_Update();
}

void Game::Finalize()
{
	delete postEffect;
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
