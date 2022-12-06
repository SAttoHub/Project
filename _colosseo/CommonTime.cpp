#include "CommonTime.h"

float CommonTime::m_Time[TIME_MAX_ELEMENT] = {};
ComPtr<ID3D12Resource> CommonTime::m_TimeConstBuffer = nullptr;

void CommonTime::Initialize()
{
	//定数バッファの生成
	DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(TimeBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_TimeConstBuffer));
}

void CommonTime::Update()
{
	TimeBuffer* TimeConstMap = nullptr;
	if (SUCCEEDED(m_TimeConstBuffer->Map(0, nullptr, (void**)&TimeConstMap))) {
		for (int i = 0; i < TIME_MAX_ELEMENT; i++) {
			m_Time[i] += 1.0f;
			if (m_Time[i] > 200000000.0f) {
				m_Time[i] = 0.0f;
			}
			TimeConstMap->Time[i] = m_Time[i]; //RGBA
		}
		m_TimeConstBuffer->Unmap(0, nullptr);
	}
}

void CommonTime::Draw(int _Index)
{
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(_Index, m_TimeConstBuffer->GetGPUVirtualAddress());
}

void CommonTime::CreateRootParameter(CD3DX12_ROOT_PARAMETER* _RootParam) {
	_RootParam->InitAsConstantBufferView(13, 0, D3D12_SHADER_VISIBILITY_ALL);//時間総合
}