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
	
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
public:
	// シェーダーをコンパイルする
	const char *LoadShader(const char*ShaderName, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
	// ShaderNameキーの付いているシェーダーを取得する
	ID3DBlob *GetShader(const char *ShaderName);
	// FileNameからシェーダーを取得して渡す。存在しない場合はエラー
	ID3DBlob *GetShaderFromFileName(LPCWSTR ShaderFileName);
	// FileNameからシェーダーを取得して渡す。存在しない場合はコンパイルして渡す。
	ID3DBlob *GetShaderAndCompile(LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);

	ShaderManager();
	~ShaderManager() {};
};

