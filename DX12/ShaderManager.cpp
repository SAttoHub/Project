#include "ShaderManager.h"

const char *ShaderManager::LoadShader(const char *ShaderName, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	// 最大数以上に作ろうとするとエラー
	if (m_Shaders.size() >= MAX_SHADER_COUNT - size_t(1)) {
		assert(("Message: Cannot generate shader that exceed MAX_SHADER_COUNT", 0));
	}

	// すでに同じシェーダーファイルを同じエントリポイントでコンパイルしていた場合はエラー
	std::unordered_map<std::string, ShaderPtr>::iterator itr = m_Shaders.begin();
	for (itr; itr != m_Shaders.end(); itr++) {
		if (itr->second.get()->ShaderFileName == FileName && itr->second.get()->EntryPointName == EntryPointName) {
			assert(("Message: That shader already exists", 0));
			exit(1);
		}
	}

	// 同じ名前のものを作ろうとするとエラー
	if (m_Shaders.find(ShaderName) != m_Shaders.end()) {
		// 存在した場合に生成失敗
		assert(("Message: A shader by that name already exists", 0));
		exit(1);
	}

	m_Shaders[ShaderName] = ShaderPtr(new Shader);
	LoadShader(&m_Shaders[ShaderName].get()->ShaderPtr, FileName, EntryPointName, ModelName);
	m_Shaders[ShaderName].get()->ShaderFileName = FileName;
	m_Shaders[ShaderName].get()->EntryPointName = EntryPointName;

	return ShaderName;
}

ID3DBlob *ShaderManager::GetShader(const char *ShaderName)
{
	if (m_Shaders.find(ShaderName) != m_Shaders.end()) {
		// 存在した場合にreturn
		return m_Shaders[ShaderName].get()->ShaderPtr.Get();
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent shader name was specified", 0));
	return nullptr;
}

ID3DBlob *ShaderManager::GetShaderFromFileName(LPCWSTR ShaderFileName)
{
	std::unordered_map<std::string, ShaderPtr>::iterator itr = m_Shaders.begin();
	for (itr; itr != m_Shaders.end(); itr++) {
		if (itr->second.get()->ShaderFileName == ShaderFileName) {
			return itr->second.get()->ShaderPtr.Get();
		}
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent shader name was specified", 0));
	return nullptr;
}

ID3DBlob *ShaderManager::GetShaderAndCompile(LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	// すでに存在する場合はそれを返す
	std::unordered_map<std::string, ShaderPtr>::iterator itr = m_Shaders.begin();
	for (itr; itr != m_Shaders.end(); itr++) {
		if (itr->second.get()->ShaderFileName == FileName && itr->second.get()->EntryPointName == EntryPointName) {
			return itr->second.get()->ShaderPtr.Get();
		}
	}


	std::wstring ws(FileName);
	//std::string Name(ws.begin(), ws.end());

	std::string Name = convString(ws);

	// 存在しなかった場合はコンパイルしてそれを返す
	m_Shaders[Name] = ShaderPtr(new Shader);
	LoadShader(&m_Shaders[Name].get()->ShaderPtr, FileName, EntryPointName, ModelName);
	m_Shaders[Name].get()->ShaderFileName = FileName;
	m_Shaders[Name].get()->EntryPointName = EntryPointName;

	return m_Shaders[Name].get()->ShaderPtr.Get();
}

ShaderManager::ShaderManager()
{
	m_Shaders.reserve(MAX_SHADER_COUNT);
}

void ShaderManager::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName) {
	HRESULT result;
	//シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		FileName, //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		EntryPointName, ModelName, //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0,
		*(&blob), &errorBlob
	);

	//シェーダーのエラー内容を表示
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

