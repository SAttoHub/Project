#pragma once
#include "DirectXBase.h"
#include "ShaderManager.h"

const int MAX_PIPELINE = 20;

enum class BlendType {
	None,
	Alpha,
	Add
};

class PipelineManager
{
	ShaderManager *Shader;//シェーダー

public:
	static int nowSetPipeline;

	int PIPELINE_NUM = 0;
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト

	//ルートシグネチャの生成
	static ComPtr<ID3D12RootSignature> rootsignature[MAX_PIPELINE];

	//パイプラインステートの生成
	static ComPtr<ID3D12PipelineState> pipelinestate[MAX_PIPELINE];

	PipelineManager(ShaderManager *Shader);
	~PipelineManager() {}

	void createPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createSpritePipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createParticlePipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *gsBlob, ID3DBlob *psBlob
	);

	void createFBXPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXBumpPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXheightPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXOutLinePipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXLinePipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *gsBlob, ID3DBlob *psBlob
	);

	void createFBXSeaPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXDepthPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXShadowDepthPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	void createFBXDOFPipeline(int PIPELINE_NUM,
		UINT inputLayoutCount,
		D3D12_INPUT_ELEMENT_DESC *inputLayout,
		ID3DBlob *vsBlob, ID3DBlob *psBlob
	);

	//int CreatePipeline(D3D12_INPUT_ELEMENT_DESC *InputLayout, ID3DBlob *vs, ID3DBlob *ps, ID3DBlob *gs, ID3DBlob *ds, ID3DBlob *hs);
};
