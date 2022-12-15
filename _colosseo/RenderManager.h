#pragma once
#include <unordered_map>
#include "Singleton.h"
#include "RenderTarget.h"

// �����_�[�^�[�Q�b�g�Ǘ�class
class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
private:
	const std::size_t MAX_RENDER_COUNT = 32;
	// �����_�[�^�[�Q�b�g�i�[�p
	typedef std::unique_ptr<RenderTarget> RenderPtr;
	std::unordered_map<std::string , RenderPtr> m_Renders;

	std::string m_NowRenderName = "";
public:
	RenderManager();
	~RenderManager();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�𐶐�����
	/// </summary>
	/// <param name="RenderName">�������郌���_�[�^�[�Q�b�g��</param>
	/// <param name="RTV_Format">RTV�̃t�H�[�}�b�g</param>
	/// <param name="IsUseDepth">�[�x���g�p���邩</param>
	void CreateRenderTarget(const char *RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution = { WINDOW_WIDTH, WINDOW_HEIGHT }, DirectX::XMFLOAT4 _ClearColor = XMFLOAT4());

	/// <summary>
	/// RenderName���w�肵�ĕ`��O�������s��
	/// </summary>
	/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
	void PreDraw(const char *RenderName, bool Clear);

	/// <summary>
	/// RenderName���w�肵�ĕ`��㏈�����s��
	/// </summary>
	/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
	void PostDraw(const char *RenderName);

	/// <summary>
	/// RenderName���w�肵�ă����_�[�e�N�X�`����ID���擾����
	/// </summary>
	/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
	int GetRenderTexture(const char *RenderName);

	/// <summary>
	/// RenderName���w�肵�Đ[�x�e�N�X�`����ID���擾����
	/// </summary>
	/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
	int GetDepthTexture(const char *RenderName);

	DirectX::XMINT2 GetNowRenderSize() {
		return m_Renders[m_NowRenderName]->GetSize();
	}
};

#pragma region RenderName �� const char* �^�Ŏw�肷��ꍇ

/// <summary>
/// �����_�[�^�[�Q�b�g�𐶐�����
/// </summary>
/// <param name="RenderName">�������郌���_�[�^�[�Q�b�g��</param>
/// <param name="RTV_Format">RTV�̃t�H�[�}�b�g</param>
/// <param name="IsUseDepth">�[�x���g�p���邩</param>
static void CreateRenderTarget(const char *RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution = { WINDOW_WIDTH, WINDOW_HEIGHT }, DirectX::XMFLOAT4 _ClearColor = XMFLOAT4()) {
	RenderManager::Instance()->CreateRenderTarget(RenderName, RTV_Format, IsUseDepth, resolution, _ClearColor);
}

/// <summary>
/// RenderName���w�肵�ĕ`��O�������s��
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static void PreDraw(const char *RenderName, bool Clear = true) {
	RenderManager::Instance()->PreDraw(RenderName, Clear);
}

/// <summary>
/// RenderName���w�肵�ĕ`��㏈�����s��
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static void PostDraw(const char *RenderName) {
	RenderManager::Instance()->PostDraw(RenderName);
}

/// <summary>
/// RenderName���w�肵�ă����_�[�e�N�X�`����ID���擾����
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static int GetRenderTexture(const char *RenderName) {
	return RenderManager::Instance()->GetRenderTexture(RenderName);
}

/// <summary>
/// RenderName���w�肵�Đ[�x�e�N�X�`����ID���擾����
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static int GetDepthTexture(const char *RenderName) {
	return RenderManager::Instance()->GetDepthTexture(RenderName);
}

#pragma endregion

#pragma region RenderName �� std::string �^�Ŏw�肷��ꍇ

/// <summary>
/// �����_�[�^�[�Q�b�g�𐶐�����
/// </summary>
/// <param name="RenderName">�������郌���_�[�^�[�Q�b�g��</param>
/// <param name="RTV_Format">RTV�̃t�H�[�}�b�g</param>
/// <param name="IsUseDepth">�[�x���g�p���邩</param>
static void CreateRenderTarget(std::string RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth) {
	RenderManager::Instance()->CreateRenderTarget(RenderName.c_str(), RTV_Format, IsUseDepth);
}

/// <summary>
/// RenderName���w�肵�ĕ`��O�������s��
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static void PreDraw(std::string RenderName, bool Clear) {
	RenderManager::Instance()->PreDraw(RenderName.c_str(), Clear);
}

/// <summary>
/// RenderName���w�肵�ĕ`��㏈�����s��
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static void PostDraw(std::string RenderName) {
	RenderManager::Instance()->PostDraw(RenderName.c_str());
}

/// <summary>
/// RenderName���w�肵�ă����_�[�e�N�X�`����ID���擾����
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static int GetRenderTexture(std::string RenderName) {
	return RenderManager::Instance()->GetRenderTexture(RenderName.c_str());
}

/// <summary>
/// RenderName���w�肵�Đ[�x�e�N�X�`����ID���擾����
/// </summary>
/// <param name="RenderName">�����_�[�^�[�Q�b�g��</param>
static int GetDepthTexture(std::string RenderName) {
	return RenderManager::Instance()->GetDepthTexture(RenderName.c_str());
}

/// <summary>
/// ���ݐݒ蒆�̃����_�[�e�N�X�`���̃T�C�Y���擾����
/// </summary>
static DirectX::XMINT2 GetNowRenderSize() {
	return RenderManager::Instance()->GetNowRenderSize();
}

#pragma endregion