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
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++) {
		adapters.emplace_back(tmpAdapter); //���I�z��ɒǉ�
	}
	for (int i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC1 adesc{};
		adapters[i]->GetDesc1(&adesc); //�A�_�v�^�[�̏����擾
		//�\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}
		std::wstring strDesc = adesc.Description; //�A�_�v�^�[��
		// Microsoft basic Render Driver, Intel UHD Graphics �����
		if (strDesc.find(L"Intel") == std::wstring::npos) {
			tmpAdapter = adapters[i]; //�̗p
			break;
		}
	}
}

void DirectXBase::CreateDeviceInAdapter() {
	for (int i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
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
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //�F���̏���
	swapchainDesc.SampleDesc.Count = 1; //�}���`�T���v�����O���Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; //�o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2; //�o�b�t�@����2�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //�t���b�v��͔j��
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


	//�e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2; //���\��2��
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//���\��2���ɂ���
	for (int i = 0; i < 2; i++) {
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//�������s������v���O�����������I��������
		if (result != S_OK) {
			exit(1);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		//�����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle
		);
	}
}

void DirectXBase::Create_Fence() {
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	//�������s������v���O�����������I��������
	if (result != S_OK) {
		exit(1);
	}
}

void DirectXBase::ClearScreen() {
	//���\�[�X�o���A�̕ύX
	Resource_Barrier_Start();
	//��ʃN���A�R�}���h
	ClearScreenCmd();
}

void DirectXBase::DoScreen() {
	ReturnResourceBarrier();
	DoCmd();
}

void DirectXBase::ClearDepthBuffer() {
	//RTV�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//�[�x�X�e���V���r���[
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
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), //�A�b�v���[�h
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);

	dsvHeapDesc.NumDescriptors = 1; //�[�x�r���[�͂ЂƂ�
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //�f�v�X�X�e���V���r���[
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//�[�x�r���[�쐬
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

	//�f�o�b�O���C���[���I����
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
	//RTV�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//�[�x�X�e���V���r���[
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//�S��ʃN���A RGBA
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXBase::ReturnResourceBarrier() {

	//�`���Ԃ���\����ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void DirectXBase::DoCmd() {
	//���߂̃N���[�Y
	cmdList->Close();
	//�R�}���h���X�g�̎��s
	ID3D12CommandList *cmdLists[] = { cmdList.Get() }; //�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//�R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator->Reset(); //�L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr); //�ĂуR�}���h���X�g�𒙂߂鏀��
	//�o�b�t�@���t���b�v
	swapchain->Present(1, 0);
}

void DirectXBase::Resource_Barrier_Start() {
	//�o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	bbIndex = swapchain->GetCurrentBackBufferIndex();
	//���\�[�X�o���A��ύX
	//�\����Ԃ���`���ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void DirectXBase::Create_Cmd_Allocator_And_List() {
	//�R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	//�������s������v���O�����������I��������
	if (result != S_OK) {
		exit(1);
	}
	//�R�}���h���X�g�𐶐�
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	//�������s������v���O�����������I��������
	if (result != S_OK) {
		exit(1);
	}
}
