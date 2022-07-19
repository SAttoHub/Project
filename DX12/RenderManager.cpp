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
	// �ő吔�ȏ�ɍ�낤�Ƃ���ƃG���[
	if (m_Renders.size() >= M_MAX_RENDER_COUNT - size_t(1)) {
		assert(("Message: Cannot generate render targets that exceed M_MAX_RENDER_COUNT", 0));
	}
	m_Renders[RenderName] = RenderPtr(new RenderTarget);
	m_Renders[RenderName]->Initialize(RTV_Format, IsUseDepth);
}

void RenderManager::PreDraw(const char *RenderName)
{
	// RenderName�L�[���t���Ă���v�f�����݂��邩�m�F
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// ���݂����ꍇ�Ɏ��s
		m_Renders[RenderName]->PreDraw();
	}
	// ���݂��Ȃ������ꍇ�G���[
	assert(("Message: A non-existent render target name was specified", 0));
}

void RenderManager::PostDraw(const char *RenderName)
{
	// RenderName�L�[���t���Ă���v�f�����݂��邩�m�F
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// ���݂����ꍇ�Ɏ��s
		m_Renders[RenderName]->PostDraw();
	}
	// ���݂��Ȃ������ꍇ�G���[
	assert(("Message: A non-existent render target name was specified", 0));
}

int RenderManager::GetRenderTexture(const char *RenderName)
{
	// RenderName�L�[���t���Ă���v�f�����݂��邩�m�F
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// ���݂����ꍇ�ɕԂ�
		return m_Renders[RenderName]->GetRenderTexture();
	}
	// ���݂��Ȃ������ꍇ�G���[
	assert(("Message: A non-existent render target name was specified", 0));
	return 0;
}

int RenderManager::GetDepthTexture(const char *RenderName)
{
	// RenderName�L�[���t���Ă���v�f�����݂��邩�m�F
	if (m_Renders.find(RenderName) != m_Renders.end()) {
		// ���݂����ꍇ�ɕԂ�
		return m_Renders[RenderName]->GetDepthTexture();
	}
	// ���݂��Ȃ������ꍇ�G���[
	assert(("Message: A non-existent render target name was specified", 0));
	return 0;
}
