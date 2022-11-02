#pragma once
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include "Window.h"
#include <wrl.h>
#include "d3dx12.h"

using namespace Microsoft::WRL;

#include "Engine\Common\Colors\ColorFuncs.h"

//�g�p����V�F�[�_�[�̎w��p
enum SHADER {
	FBXSHADER,
	FBX_Bump_SHADER,
	FBX_height_SHADER,
	FBX_Line_SHADER, // �ӂ̂�
	Sea_SHADER, // �C�e�X�g
	Depth_SHEADER, // �[�x�l�p
	Shadow_Depth_SHEADER,
	DOF_SHEADER, // �[�x�l�p

	FBXSHADER_INS,
	Depth_SHEADER_INS,
	Shadow_Depth_SHEADER_INS,
	DOF_SHEADER_INS
};


struct DirectXBase {
public:
	DirectXBase() {
		result = HRESULT();
	};
	~DirectXBase() {
		//fence.Reset();
		//cmdAllocator.Reset();
		//cmdList.Reset();
		//cmdQueue.Reset();

		//ID3D12DebugDevice *debugInterface;
		//if (SUCCEEDED(DirectXBase::dev.Get()->QueryInterface(&debugInterface))) {
		//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		//	debugInterface->Release();
		//}
	}

	//��ʃN���A�p�̕ϐ�(�w�i�F)
	float clearColor[4] = { 0.153f, 0.1215f, 0.2157f, 0.0f };
	//�E�B���h�E�̑傫����ۑ����Ă���
	static float Win_Width;
	static float Win_Height;
	static Window *win;

	HRESULT result;
	static ComPtr<ID3D12Device> dev;
	static ComPtr<IDXGIFactory6> dxgiFactory;
	static ComPtr<IDXGISwapChain4> swapchain;
	static ComPtr<ID3D12CommandAllocator> cmdAllocator;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	static ComPtr<ID3D12CommandQueue> cmdQueue;
	static ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	static std::vector<ComPtr<IDXGIAdapter1>> adapters;
	static ComPtr<IDXGIAdapter1> tmpAdapter;
	static D3D_FEATURE_LEVEL levels[4];
	static D3D_FEATURE_LEVEL featureLevel;
	static D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;
	static DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
	static D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	static std::vector< ComPtr<ID3D12Resource>> backBuffers;
	static ComPtr<ID3D12Fence> fence;
	static UINT64 fenceVal;
	static UINT bbIndex;
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	static CD3DX12_RESOURCE_DESC depthResDesc;
	static ComPtr<ID3D12Resource> depthBuffer;
	static D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc; //�[�x�r���[�p�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> dsvHeap;
	static D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	//static ComPtr<ID3D12Debug> debugController;

	/*-----------------�O���t�B�b�N�{�[�h�̃A�_�v�^���-----------------*/
	void ADDAdapters();

	/*-----------------�f�o�C�X�̐���-----------------*/
	void CreateDeviceInAdapter();

	/*-----------------�R�}���h�A���P�[�^�ƃR�}���h���X�g�̐���-----------------*/
	void Create_Cmd_Allocator_And_List();

	/*-----------------�R�}���h�L���[�̐���-----------------*/
	void Create_Cmd_Queue();

	/*-----------------�X���b�v�`�F�[���̐���-----------------*/
	void Create_Swap_Chain(Window *Win);

	/*-----------------�f�B�X�N���v�^�q�[�v��RTV(�����_�[�^�[�Q�b�g�r���[)�̐���-----------------*/
	void Create_Des_And_RTV();

	/*-----------------�t�F���X�̐���-----------------*/
	void Create_Fence();



	//�������烁�C�����[�v���ɋL�q������
	/*-----------------���\�[�X�o���A-----------------*/
	void Resource_Barrier_Start();
	//��ʃN���A�R�}���h
	void ClearScreenCmd();
	//���\�[�X�o���A�̕��A
	void ReturnResourceBarrier();

	/*-----------------�R�}���h�̃t���b�V��-----------------*/
	void DoCmd();


	//�[�x�o�b�t�@
	void depthCreate();


	//�������܂Ƃ�
	void DirectXReset(Window *Win);
	//���C�����[�v�̍ŏ���
	void ClearScreen();
	//�Q�[�����[�v�̏I����
	void DoScreen();

	//�[�x�o�b�t�@�̃N���A
	static void ClearDepthBuffer();
};