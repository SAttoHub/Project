#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

void ShaderManager::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName) {
	HRESULT result;
	//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		FileName, //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		EntryPointName, ModelName, //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0,
		*(&blob), &errorBlob
	);

	//�V�F�[�_�[�̃G���[���e��\��
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
	//*blob = _blob;
}

void ShaderManager::LoadShaderAll()
{
	LoadShader(&Sprite_vsBlob, L"Resource/shader/SpriteVertexShader.hlsl", "VSmain", "vs_5_0");
	LoadShader(&Sprite_psBlob, L"Resource/shader/SpritePixelShader.hlsl", "PSmain", "ps_5_0");
	LoadShader(&Basic_vsBlob, L"Resource/shader/BasicVertexShader.hlsl", "VSmain", "vs_5_0");
	LoadShader(&Basic_psBlob, L"Resource/shader/BasicPixelShader.hlsl", "PSmain", "ps_5_0");
	LoadShader(&Material_vsBlob, L"Resource/shader/OBJVertexShader.hlsl", "main", "vs_5_0");
	LoadShader(&Material_psBlob, L"Resource/shader/OBJPixelShader.hlsl", "main", "ps_5_0");

	LoadShader(&Particle_vsBlob, L"Resource/shader/ParticleVertexShader.hlsl", "main", "vs_5_0");
	LoadShader(&Particle_gsBlob, L"Resource/shader/ParticleGeometryShader.hlsl", "main", "gs_5_0");
	LoadShader(&Particle_psBlob, L"Resource/shader/ParticlePixelShader.hlsl", "main", "ps_5_0");

	LoadShader(&FBX_vsBlob, L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");
	LoadShader(&FBX_psBlob, L"Resource/shader/FBXPS.hlsl", "main", "ps_5_0");
	LoadShader(&FBX_Bump_psBlob, L"Resource/shader/FBXBumpPS.hlsl", "main", "ps_5_0");
	LoadShader(&FBX_height_psBlob, L"Resource/shader/FBXHeightPS.hlsl", "main", "ps_5_0");
	LoadShader(&FBX_OutLine_psBlob, L"Resource/shader/FBXOutLinePS.hlsl", "main", "ps_5_0");

	LoadShader(&FBX_Line_gsBlob, L"Resource/shader/FBXLineGS.hlsl", "main", "gs_5_0");
	LoadShader(&FBX_Line_psBlob, L"Resource/shader/FBXLinePS.hlsl", "main", "ps_5_0");
	LoadShader(&FBX_Sea_psBlob, L"Resource/shader/SeaPS.hlsl", "main", "ps_5_0");
}