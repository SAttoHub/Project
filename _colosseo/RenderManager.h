#pragma once
#include <unordered_map>
#include "Singleton.h"
#include "RenderTarget.h"

// レンダーターゲット管理class
class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
private:
	const std::size_t MAX_RENDER_COUNT = 32;
	// レンダーターゲット格納用
	typedef std::unique_ptr<RenderTarget> RenderPtr;
	std::unordered_map<std::string , RenderPtr> m_Renders;

	std::string m_NowRenderName = "";
public:
	RenderManager();
	~RenderManager();

	/// <summary>
	/// レンダーターゲットを生成する
	/// </summary>
	/// <param name="RenderName">生成するレンダーターゲット名</param>
	/// <param name="RTV_Format">RTVのフォーマット</param>
	/// <param name="IsUseDepth">深度を使用するか</param>
	void CreateRenderTarget(const char *RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution = { WINDOW_WIDTH, WINDOW_HEIGHT }, DirectX::XMFLOAT4 _ClearColor = XMFLOAT4());

	/// <summary>
	/// RenderNameを指定して描画前処理を行う
	/// </summary>
	/// <param name="RenderName">レンダーターゲット名</param>
	void PreDraw(const char *RenderName, bool Clear);

	/// <summary>
	/// RenderNameを指定して描画後処理を行う
	/// </summary>
	/// <param name="RenderName">レンダーターゲット名</param>
	void PostDraw(const char *RenderName);

	/// <summary>
	/// RenderNameを指定してレンダーテクスチャのIDを取得する
	/// </summary>
	/// <param name="RenderName">レンダーターゲット名</param>
	int GetRenderTexture(const char *RenderName);

	/// <summary>
	/// RenderNameを指定して深度テクスチャのIDを取得する
	/// </summary>
	/// <param name="RenderName">レンダーターゲット名</param>
	int GetDepthTexture(const char *RenderName);

	DirectX::XMINT2 GetNowRenderSize() {
		return m_Renders[m_NowRenderName]->GetSize();
	}
};

#pragma region RenderName を const char* 型で指定する場合

/// <summary>
/// レンダーターゲットを生成する
/// </summary>
/// <param name="RenderName">生成するレンダーターゲット名</param>
/// <param name="RTV_Format">RTVのフォーマット</param>
/// <param name="IsUseDepth">深度を使用するか</param>
static void CreateRenderTarget(const char *RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution = { WINDOW_WIDTH, WINDOW_HEIGHT }, DirectX::XMFLOAT4 _ClearColor = XMFLOAT4()) {
	RenderManager::Instance()->CreateRenderTarget(RenderName, RTV_Format, IsUseDepth, resolution, _ClearColor);
}

/// <summary>
/// RenderNameを指定して描画前処理を行う
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static void PreDraw(const char *RenderName, bool Clear = true) {
	RenderManager::Instance()->PreDraw(RenderName, Clear);
}

/// <summary>
/// RenderNameを指定して描画後処理を行う
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static void PostDraw(const char *RenderName) {
	RenderManager::Instance()->PostDraw(RenderName);
}

/// <summary>
/// RenderNameを指定してレンダーテクスチャのIDを取得する
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static int GetRenderTexture(const char *RenderName) {
	return RenderManager::Instance()->GetRenderTexture(RenderName);
}

/// <summary>
/// RenderNameを指定して深度テクスチャのIDを取得する
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static int GetDepthTexture(const char *RenderName) {
	return RenderManager::Instance()->GetDepthTexture(RenderName);
}

#pragma endregion

#pragma region RenderName を std::string 型で指定する場合

/// <summary>
/// レンダーターゲットを生成する
/// </summary>
/// <param name="RenderName">生成するレンダーターゲット名</param>
/// <param name="RTV_Format">RTVのフォーマット</param>
/// <param name="IsUseDepth">深度を使用するか</param>
static void CreateRenderTarget(std::string RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth) {
	RenderManager::Instance()->CreateRenderTarget(RenderName.c_str(), RTV_Format, IsUseDepth);
}

/// <summary>
/// RenderNameを指定して描画前処理を行う
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static void PreDraw(std::string RenderName, bool Clear) {
	RenderManager::Instance()->PreDraw(RenderName.c_str(), Clear);
}

/// <summary>
/// RenderNameを指定して描画後処理を行う
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static void PostDraw(std::string RenderName) {
	RenderManager::Instance()->PostDraw(RenderName.c_str());
}

/// <summary>
/// RenderNameを指定してレンダーテクスチャのIDを取得する
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static int GetRenderTexture(std::string RenderName) {
	return RenderManager::Instance()->GetRenderTexture(RenderName.c_str());
}

/// <summary>
/// RenderNameを指定して深度テクスチャのIDを取得する
/// </summary>
/// <param name="RenderName">レンダーターゲット名</param>
static int GetDepthTexture(std::string RenderName) {
	return RenderManager::Instance()->GetDepthTexture(RenderName.c_str());
}

/// <summary>
/// 現在設定中のレンダーテクスチャのサイズを取得する
/// </summary>
static DirectX::XMINT2 GetNowRenderSize() {
	return RenderManager::Instance()->GetNowRenderSize();
}

#pragma endregion