#include "Framework.h"

void Framework::Initialize()
{
    FbxLoader::GetInstance()->Initialize();
    //最大FPS指定
    fps = new FPS(60);

    //メインループで使う変数等の宣言
    win = new Window(); //ウィンドウの作成

    //DirectX初期化
    DXBase.DirectXReset(win);
    audio.XAudioInit();
    Pipeline = new PipelineManager(&Shader);

    Cam = new Camera(win);

    DX.DirectX3DObjectReset(win);
    Tex.TextureDescHeapCreate();
    LightGroup::StaticInitialize();
    //プリミティブ
    Primitive2D::Instance()->Initialize();
    Primitive3D::Instance()->Initialize();

    //DirectInupt初期化---------------
    Input input;
    input.Initialize(win);
    //-------------------------------
}

void Framework::Finalize()
{
    FbxLoader::GetInstance()->Finalize();
    //精度を戻す
    timeBeginPeriod(1);
    //ウィンドウクラスの解放
    delete win;
}
