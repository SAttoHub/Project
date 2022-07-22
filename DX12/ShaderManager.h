#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include <unordered_map>

struct Shader {
	ComPtr<ID3DBlob> ShaderPtr;
	LPCWSTR ShaderFileName;
	LPCSTR EntryPointName;
};

class ShaderManager {
private:
	const std::size_t MAX_SHADER_COUNT = 64;
	typedef std::unique_ptr<Shader> ShaderPtr;
	std::unordered_map<std::string, ShaderPtr> m_Shaders;
	
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
public:
	// �V�F�[�_�[���R���p�C������
	const char *LoadShader(const char*ShaderName, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
	// ShaderName�L�[�̕t���Ă���V�F�[�_�[���擾����
	ID3DBlob *GetShader(const char *ShaderName);
	// FileName����V�F�[�_�[���擾���ēn���B���݂��Ȃ��ꍇ�̓G���[
	ID3DBlob *GetShaderFromFileName(LPCWSTR ShaderFileName);
	// FileName����V�F�[�_�[���擾���ēn���B���݂��Ȃ��ꍇ�̓R���p�C�����ēn���B
	ID3DBlob *GetShaderAndCompile(LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);

	ShaderManager();
	~ShaderManager() {};
};

