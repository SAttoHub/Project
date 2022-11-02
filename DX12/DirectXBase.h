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

//使用するシェーダーの指定用
enum SHADER {
	FBXSHADER,
	FBX_Bump_SHADER,
	FBX_height_SHADER,
	FBX_Line_SHADER, // 辺のみ
	Sea_SHADER, // 海テスト
	Depth_SHEADER, // 深度値用
	Shadow_Depth_SHEADER,
	DOF_SHEADER, // 深度値用

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

	//画面クリア用の変数(背景色)
	float clearColor[4] = { 0.153f, 0.1215f, 0.2157f, 0.0f };
	//ウィンドウの大きさを保存しておく
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
	static D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc; //深度ビュー用デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> dsvHeap;
	static D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	//static ComPtr<ID3D12Debug> debugController;

	/*-----------------グラフィックボードのアダプタを列挙-----------------*/
	void ADDAdapters();

	/*-----------------デバイスの生成-----------------*/
	void CreateDeviceInAdapter();

	/*-----------------コマンドアロケータとコマンドリストの生成-----------------*/
	void Create_Cmd_Allocator_And_List();

	/*-----------------コマンドキューの生成-----------------*/
	void Create_Cmd_Queue();

	/*-----------------スワップチェーンの生成-----------------*/
	void Create_Swap_Chain(Window *Win);

	/*-----------------ディスクリプタヒープとRTV(レンダーターゲットビュー)の生成-----------------*/
	void Create_Des_And_RTV();

	/*-----------------フェンスの生成-----------------*/
	void Create_Fence();



	//ここからメインループ内に記述するやつ
	/*-----------------リソースバリア-----------------*/
	void Resource_Barrier_Start();
	//画面クリアコマンド
	void ClearScreenCmd();
	//リソースバリアの復帰
	void ReturnResourceBarrier();

	/*-----------------コマンドのフラッシュ-----------------*/
	void DoCmd();


	//深度バッファ
	void depthCreate();


	//初期化まとめ
	void DirectXReset(Window *Win);
	//メインループの最初に
	void ClearScreen();
	//ゲームループの終わりに
	void DoScreen();

	//深度バッファのクリア
	static void ClearDepthBuffer();
};