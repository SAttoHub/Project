#include "Framework.h"

void Framework::Initialize()
{
    FbxLoader::GetInstance()->Initialize();
    //�ő�FPS�w��
    fps = new FPS(244);

    //���C�����[�v�Ŏg���ϐ����̐錾
    win = new Window(); //�E�B���h�E�̍쐬

    //DirectX������
    DXBase.DirectXReset(win);
    audio.XAudioInit();
    Pipeline = new PipelineManager(&Shader);
    //static DWORD dwID;
  //  CreateThread(NULL, 0, T_Test, &Shader, 0, &dwID);
    DirectX3dObject::CreateInstancePipiline(&Shader);
    Cam = new Camera(win);

    DX.DirectX3DObjectReset(win);
    Tex.TextureDescHeapCreate();
    LightGroup::StaticInitialize();
    //�v���~�e�B�u
    Primitive2D::Instance()->Initialize();
    Primitive3D::Instance()->Initialize();

    //DirectInupt������---------------
    Input input;
    input.Initialize(win);
    //-------------------------------
}

void Framework::Finalize()
{
    FbxLoader::GetInstance()->Finalize();
    //���x��߂�
    timeBeginPeriod(1);
    //�E�B���h�E�N���X�̉��
    delete win;
}
