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
	//�}�E�X�J�[�\����\�����Ȃ�
	CursorShow(true);

	postEffect = new PostEffect;
	postEffect->Initialize();

	GaussianEffect = new Gaussian;
	GaussianEffect->Initialize();
}

void Game::Update()
{
    //-----------------��ʂ̃N���A-----------------//
    DXBase.ClearScreen();

    //-----------------�L�[�{�[�h�̓��͏�Ԃ̍X�V-----------------//
    Input::Update();
    // WinAPIInput::Update();
    Primitive2D::Instance()->Update();
    Primitive3D::Instance()->Update();

    //-----------------��������v���O�����L��-----------------//
    game->Update();
    DX.AllObjectUpdate();

	Primitive2D::Instance()->BackDraw();

	postEffect->PreDrawScene();
    //Drawobject3d(skydome);
    game->Draw();

    //�f�o�b�O�e�L�X�g��Draw
    DXBase.ClearDepthBuffer();

	postEffect->PostDrawScene();

	//GaussianEffect->PreDrawScene();

	postEffect->Draw();

	
	//GaussianEffect->PostDrawScene();
	//GaussianEffect->Draw(postEffect->descHeapSRV.Get());
	

	//�X�v���C�g�֘A�`��
	Primitive2D::Instance()->Draw();
	//SP.SpriteDraw(&CenterDot);

    //-----------------�����܂Ńv���O�����L��-----------------//

    //-----------------���߂̎��s�ƃo�b�t�@�̃t���b�v-----------------//
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
