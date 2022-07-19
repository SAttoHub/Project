#include "RenderManager.h"
#include <cassert>

RenderManager::RenderManager()
{
	m_Renders.reserve(M_MAX_RENDER_COUNT);
}

RenderManager::~RenderManager()
{
}

void RenderManager::CreateRenderTarget(const char *RenderName, DXGI_FORMAT RTV_Format, bool IsUseDepth)
{
	// 最大数以上に作ろうとするとエラー
	if (m_Renders.size() >= M_MAX_RENDER_COUNT - size_t(1)) {
		assert(("Message: Cannot generate render targets that exceed M_MAX_RENDER_COUNT", 0));
	}
	m_Renders[RenderName] = RenderPtr(new RenderTarget);
	m_Renders[RenderName]->Initialize(RTV_Format, IsUseDepth);
}

void RenderManager::PreDraw(const char *RenderName)
{
	// RenderNameキーが付いている要素が存在するか確認
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// 存在した場合に実行
		m_Renders[RenderName]->PreDraw();
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent render target name was specified", 0));
}

void RenderManager::PostDraw(const char *RenderName)
{
	// RenderNameキーが付いている要素が存在するか確認
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// 存在した場合に実行
		m_Renders[RenderName]->PostDraw();
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent render target name was specified", 0));
}

int RenderManager::GetRenderTexture(const char *RenderName)
{
	// RenderNameキーが付いている要素が存在するか確認
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// 存在した場合に返す
		return m_Renders[RenderName]->GetRenderTexture();
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent render target name was specified", 0));
	return 0;
}

int RenderManager::GetDepthTexture(const char *RenderName)
{
	// RenderNameキーが付いている要素が存在するか確認
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// 存在した場合に返す
		return m_Renders[RenderName]->GetDepthTexture();
	}
	// 存在しなかった場合エラー
	assert(("Message: A non-existent render target name was specified", 0));
	return 0;
}
