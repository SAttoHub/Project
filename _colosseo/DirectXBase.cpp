#include "DirectXBase.h"

float DirectXBase::Win_Width;
float DirectXBase::Win_Height;
Window *DirectXBase::win;
ComPtr<ID3D12Device> DirectXBase::dev;
ComPtr<IDXGIFactory6> DirectXBase::dxgiFactory;
ComPtr<IDXGISwapChain4> DirectXBase::swapchain;
ComPtr<ID3D12CommandAllocator> DirectXBase::cmdAllocator;
ComPtr<ID3D12GraphicsCommandList> DirectXBase::cmdList;
ComPtr<ID3D12CommandQueue> DirectXBase::cmdQueue;
ComPtr<ID3D12DescriptorHeap> DirectXBase::rtvHeaps;
std::vector<ComPtr<IDXGIAdapter1>> DirectXBase::adapters;
ComPtr<IDXGIAdapter1> DirectXBase::tmpAdapter;
D3D_FEATURE_LEVEL DirectXBase::levels[4] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};
D3D_FEATURE_LEVEL DirectXBase::featureLevel;
D3D12_COMMAND_QUEUE_DESC DirectXBase::cmdQueueDesc{};
DXGI_SWAP_CHAIN_DESC1 DirectXBase::swapchainDesc{};
D3D12_DESCRIPTOR_HEAP_DESC DirectXBase::heapDesc{};
std::vector< ComPtr<ID3D12Resource>> DirectXBase::backBuffers;
ComPtr<ID3D12Fence> DirectXBase::fence;
UINT64 DirectXBase::fenceVal = 0;
UINT DirectXBase::bbIndex;
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::rtvH;
CD3DX12_RESOURCE_DESC DirectXBase::depthResDesc{};
ComPtr<ID3D12Resource> DirectXBase::depthBuffer;
D3D12_DESCRIPTOR_HEAP_DESC DirectXBase::dsvHeapDesc{};
ComPtr<ID3D12DescriptorHeap> DirectXBase::dsvHeap;
D3D12_DEPTH_STENCIL_VIEW_DESC DirectXBase::dsvDesc = {};

//ComPtr<ID3D12Debug> DirectXBase::debugController;


void DirectXBase::ADDAdapters() {
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++) {
		adapters.emplace_back(tmpAdapter); //動的配列に追加
	}
	for (int i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC1 adesc{};
		adapters[i]->GetDesc1(&adesc); //アダプターの情報を取得
		//ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}
		std::wstring strDesc = adesc.Description; //アダプター名
		// Microsoft basic Render Driver, Intel UHD Graphics を回避
		if (strDesc.find(L"Intel") == std::wstring::npos) {
			tmpAdapter = adapters[i]; //採用
			break;
		}
	}
}

void DirectXBase::CreateDeviceInAdapter() {
	for (int i = 0; i < _countof(levels); i++) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXBase::Create_Cmd_Queue() {
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXBase::Create_Swap_Chain(Window *Win) {
	swapchainDesc.Width = Win->Width;
	swapchainDesc.Height = Win->Height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式
	swapchainDesc.SampleDesc.Count = 1; //マルチサンプリングしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; //バックバッファ用
	swapchainDesc.BufferCount = 2; //バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ComPtr<IDXGISwapChain1> swapchain1;
	dxgiFactory->CreateSwapChainForHwnd(cmdQueue.Get(),
		Win->WinHandle,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);
	swapchain1.As(&swapchain);
}

void DirectXBase::Create_Des_And_RTV() {
	ComPtr<ID3D12Resource> a;
	backBuffers.emplace_back(a);
	backBuffers.emplace_back(a);


	//各種設定をしてディスクリプタヒープを生成
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー
	heapDesc.NumDescriptors = 2; //裏表の2つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//裏表の2つ分について
	for (int i = 0; i < 2; i++) {
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//生成失敗したらプログラムを強制終了させる
		if (result != S_OK) {
			exit(1);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle
		);
	}
}

void DirectXBase::Create_Fence() {
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	//生成失敗したらプログラムを強制終了させる
	if (result != S_OK) {
		exit(1);
	}
}

void DirectXBase::ClearScreen() {
	//リソースバリアの変更
	Resource_Barrier_Start();
	//画面クリアコマンド
	ClearScreenCmd();
}

void DirectXBase::DoScreen() {
	ReturnResourceBarrier();
	DoCmd();
}

void DirectXBase::ClearDepthBuffer() {
	//RTV用ディスクリプタヒープのハンドルを取得
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//深度ステンシルビュー
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXBase::depthCreate() {
	depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), //アップロード
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);

	dsvHeapDesc.NumDescriptors = 1; //深度ビューはひとつ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //デプスステンシルビュー
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//深度ビュー作成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXBase::DirectXReset(Window *Win) {
	this->win = Win;
	Win_Width = float(Win->Width);
	Win_Height = float(Win->Height);
	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))){
	//		debugController->EnableDebugLayer();
	//}

	//デバッグレイヤーをオンに
	//ComPtr<ID3D12Debug1> debugController;
	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	//{
	//	debugController->EnableDebugLayer();
	//	debugController->SetEnableGPUBasedValidation(TRUE);
	//}

	ADDAdapters();
	CreateDeviceInAdapter();
	Create_Cmd_Allocator_And_List();
	Create_Cmd_Queue();
	Create_Swap_Chain(Win);
	Create_Des_And_RTV();
	depthCreate();
	Create_Fence();
}

void DirectXBase::ClearScreenCmd() {
	//RTV用ディスクリプタヒープのハンドルを取得
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//深度ステンシルビュー
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//全画面クリア RGBA
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXBase::ReturnResourceBarrier() {

	//描画状態から表示状態に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void DirectXBase::DoCmd() {
	//命令のクローズ
	cmdList->Close();
	//コマンドリストの実行
	ID3D12CommandList *cmdLists[] = { cmdList.Get() }; //コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//コマンドリストの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator->Reset(); //キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr); //再びコマンドリストを貯める準備
	//バッファをフリップ
	swapchain->Present(1, 0);
}

void DirectXBase::Resource_Barrier_Start() {
	//バックバッファの番号を取得(2つなので0番か1番)
	bbIndex = swapchain->GetCurrentBackBufferIndex();
	//リソースバリアを変更
	//表示状態から描画状態に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void DirectXBase::Create_Cmd_Allocator_And_List() {
	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	//生成失敗したらプログラムを強制終了させる
	if (result != S_OK) {
		exit(1);
	}
	//コマンドリストを生成
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	//生成失敗したらプログラムを強制終了させる
	if (result != S_OK) {
		exit(1);
	}
}
