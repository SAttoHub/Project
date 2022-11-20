#include "ShaderManager.h"

const char *ShaderManager::LoadShader(const char *ShaderName, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	// �ő吔�ȏ�ɍ�낤�Ƃ���ƃG���[
	if (m_Shaders.size() >= MAX_SHADER_COUNT - size_t(1)) {
		assert(("Message: Cannot generate shader that exceed MAX_SHADER_COUNT", 0));
	}

	// ���łɓ����V�F�[�_�[�t�@�C���𓯂��G���g���|�C���g�ŃR���p�C�����Ă����ꍇ�̓G���[
	std::unordered_map<std::string, ShaderPtr>::iterator itr = m_Shaders.begin();
	for (itr; itr != m_Shaders.end(); itr++) {
		if (itr->second.get()->ShaderFileName == FileName && itr->second.get()->EntryPointName == EntryPointName) {
			assert(("Message: That shader already exists", 0));
			exit(1);
		}
	}

	// �������O�̂��̂���낤�Ƃ���ƃG���[
	if (m_Shaders.find(ShaderName) != m_Shaders.end()) {
		// ���݂����ꍇ�ɐ������s
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
		// ���݂����ꍇ��return
		return m_Shaders[ShaderName].get()->ShaderPtr.Get();
	}
	// ���݂��Ȃ������ꍇ�G���[
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
	// ���݂��Ȃ������ꍇ�G���[
	assert(("Message: A non-existent shader name was specified", 0));
	return nullptr;
}

ID3DBlob *ShaderManager::GetShaderAndCompile(LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	// ���łɑ��݂���ꍇ�͂����Ԃ�
	std::unordered_map<std::string, ShaderPtr>::iterator itr = m_Shaders.begin();
	for (itr; itr != m_Shaders.end(); itr++) {
		if (itr->second.get()->ShaderFileName == FileName && itr->second.get()->EntryPointName == EntryPointName) {
			return itr->second.get()->ShaderPtr.Get();
		}
	}


	std::wstring ws(FileName);
	//std::string Name(ws.begin(), ws.end());

	std::string Name = convString(ws);

	// ���݂��Ȃ������ꍇ�̓R���p�C�����Ă����Ԃ�
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
}

