#include "imguiCommon.h"
#include "DirectXBase.h"

imguiCommon::imguiCommon()
{
	// SRV_CBV_UAV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT result = DirectXBase::dev->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(heap.GetAddressOf()));

	if (heap == nullptr)assert(0);

	if (ImGui::CreateContext() == nullptr)assert(0);

	bool blnResult = ImGui_ImplWin32_Init(DirectXBase::win->WinHandle);
	if (!blnResult)assert(0);

	blnResult = ImGui_ImplDX12_Init(
		DirectXBase::dev.Get(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM,	//書き込み先のRTVのフォーマット
		heap.Get(),
		heap.Get()->GetCPUDescriptorHandleForHeapStart(),	//CPUハンドル
		heap.Get()->GetGPUDescriptorHandleForHeapStart());	//GPUハンドル
}

imguiCommon::~imguiCommon()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void imguiCommon::CommandExcute(bool Flag)
{
	if (Flag){
		if (!commandlist.empty()) {
			for (auto command : commandlist) {
				command();
			}
			commandlist.clear();
		}
		ImGui::Render();
		DirectXBase::cmdList->SetDescriptorHeaps(1, heap.GetAddressOf());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXBase::cmdList.Get());
	}
	else {
		if (!commandlist.empty())commandlist.clear();
	}
}