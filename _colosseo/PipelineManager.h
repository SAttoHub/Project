#pragma once
#include "DirectXBase.h"
#include "ShaderManager.h"

const int MAX_PIPELINE = MAX_PIPLINE;

enum class BlendMode {
	BM_NONE,
	BM_ALPHA,
	BM_ADD,
	BM_SUB
};

class PipelineManager
{
	ShaderManager *Shader;//シェーダー

	static int m_NowSetPipeline;

	static int PIPELINE_NUM;
	static ComPtr<ID3DBlob> m_ErrorBlob; //エラーオブジェクト

	//ルートシグネチャの生成
	static ComPtr<ID3D12RootSignature> m_RootSignature[MAX_PIPELINE];
	//パイプラインステートの生成
	static ComPtr<ID3D12PipelineState> m_PipelineState[MAX_PIPELINE];
public:

	PipelineManager(ShaderManager *Shader);
	~PipelineManager() {}

	/// <summary>
	/// パイプラインの生成
	/// </summary>
	/// <param name="_PIPELINE_NUM"> パイプラインを生成するインデックス　</param>
	/// <param name="_BlendMode">　ブレンドモード　</param>
	/// <param name="_CullMode">　カリングモード　</param>
	/// <param name="_InputLayoutCount">　インプットレイアウト配列の要素数_countof(_InputLayout) </param>
	/// <param name="_InputLayout">　インプットレイアウト配列　</param>
	/// <param name="_RootParamCount">　ルートパラメータ配列の要素数_countof(_RootParams)　</param>
	/// <param name="_RootParams">　ルートパラメータ配列　</param>
	/// <param name="_RTVFormat">　レンダーターゲットビューのフォーマット　</param>
	/// <param name="_vsBlob">　頂点シェーダー　</param>
	/// <param name="_psBlob">　ピクセルシェーダー　</param>
	/// <param name="_gsBlob">　ジオメトリシェーダー　</param>
	static void CreatePipeline(int _PIPELINE_NUM, BlendMode _BlendMode, D3D12_CULL_MODE _CullMode,
		int _InputLayoutCount, D3D12_INPUT_ELEMENT_DESC* _InputLayout,
		int _RootParamCount, CD3DX12_ROOT_PARAMETER* _RootParams,
		DXGI_FORMAT _RTVFormat,
		ID3DBlob* _vsBlob, ID3DBlob* _psBlob, ID3DBlob* _gsBlob = nullptr);

	/*----- 使用する時にコードが見にくくなったので別に分けた -----*/

	static void createInstancePipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		ID3DBlob* vsBlob, ID3DBlob* psBlob
	);

	static void createInstanceDepthPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		ID3DBlob* vsBlob, ID3DBlob* psBlob
	);

	static void createInstanceShadowDepthPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		ID3DBlob* vsBlob, ID3DBlob* psBlob
	);

	static void createInstanceDOFPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		ID3DBlob* vsBlob, ID3DBlob* psBlob
	);


public:
	// インデックスのルートシグネチャを取得
	static ID3D12RootSignature* GetRootSignature(int Index) {
		return m_RootSignature[Index].Get();
	}
	// インデックスのパイプラインステートを取得
	static ID3D12PipelineState* GetPipelineState(int Index) {
		return m_PipelineState[Index].Get();
	}
};