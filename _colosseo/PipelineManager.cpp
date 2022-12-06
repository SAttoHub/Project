#include "PipelineManager.h"
int PipelineManager::m_NowSetPipeline = -1;
int PipelineManager::PIPELINE_NUM = 0;
ComPtr<ID3DBlob> PipelineManager::m_ErrorBlob;
ComPtr<ID3D12RootSignature> PipelineManager::m_RootSignature[MAX_PIPELINE];
ComPtr<ID3D12PipelineState> PipelineManager::m_PipelineState[MAX_PIPELINE];

PipelineManager::PipelineManager(ShaderManager *Shader) {
	this->Shader = Shader;

	//頂点シェーダーに渡すための頂点データを整える
	D3D12_INPUT_ELEMENT_DESC InputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEINDICES", 0, DXGI_FORMAT_R16G16B16A16_SINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEINDICESB", 0, DXGI_FORMAT_R16G16B16A16_SINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHTSB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
	// FBX
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[5] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		CreatePipeline(FBXSHADER, BlendMode::BM_ALPHA, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/FBXPS.hlsl", "main", "ps_5_0"));
	}
	// FBX_Bump
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0レジスタ
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV2; //テクスチャ用
		descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); //t1レジスタ
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[6] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);
		CreatePipeline(FBX_Bump_SHADER, BlendMode::BM_NONE, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/FBXBumpPS.hlsl", "main", "ps_5_0"));
	}
	// FBX_height
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0レジスタ
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV2; //テクスチャ用
		descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); //t1レジスタ
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV3; //テクスチャ用
		descRangeSRV3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); //t2レジスタ
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[7] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[6].InitAsDescriptorTable(1, &descRangeSRV3, D3D12_SHADER_VISIBILITY_ALL);
		CreatePipeline(FBX_height_SHADER, BlendMode::BM_NONE, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/FBXHeightPS.hlsl", "main", "ps_5_0"));
	}
	// FBX_Line
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[5] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		CreatePipeline(FBX_Line_SHADER, BlendMode::BM_NONE, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/FBXLinePS.hlsl", "main", "ps_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/FBXLineGS.hlsl", "main", "gs_5_0"));
	}
	// FBX_Sea
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[6] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
		CreatePipeline(Sea_SHADER, BlendMode::BM_ALPHA, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/SeaPS.hlsl", "main", "ps_5_0"));
	}
	// FBX_Depth
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[7] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
		rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
		CreatePipeline(Depth_SHEADER, BlendMode::BM_ALPHA, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R32_FLOAT,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/Depth2.hlsl", "main", "ps_5_0"));
	}
	// FBX_ShadowDepth
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[7] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
		rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
		CreatePipeline(Shadow_Depth_SHEADER, BlendMode::BM_ALPHA, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R32_FLOAT,
			Shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_VS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_PS.hlsl", "main", "ps_5_0"));
	}
	// FBX_DOF_Depth
	{
		//デスクリプタテーブルの設定
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//ルートパラメータの設定
		CD3DX12_ROOT_PARAMETER rootparams[7] = {};
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
		rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
		rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
		CreatePipeline(DOF_SHEADER, BlendMode::BM_ALPHA, D3D12_CULL_MODE_BACK,
			_countof(InputLayout), InputLayout,
			_countof(rootparams), rootparams,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
			Shader->GetShaderAndCompile(L"Resource/shader/DepthPS.hlsl", "main", "ps_5_0"));
	}
}


void PipelineManager::CreatePipeline(int _PIPELINE_NUM, BlendMode _BlendMode, D3D12_CULL_MODE _CullMode,
	int _InputLayoutCount, D3D12_INPUT_ELEMENT_DESC* _InputLayout,
	int _RootParamCount, CD3DX12_ROOT_PARAMETER* _RootParams,
	DXGI_FORMAT _RTVFormat,
	ID3DBlob* _vsBlob, ID3DBlob* _psBlob, ID3DBlob* _gsBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(_vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(_psBlob);
	if (_gsBlob != nullptr) {
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(_gsBlob);
	}
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = _CullMode;
	//ブレンドステートの設定
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	switch (_BlendMode)
	{
	case BlendMode::BM_NONE:
		blenddesc.BlendEnable = false;
		break;
	case BlendMode::BM_ALPHA:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	case BlendMode::BM_ADD:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::BM_SUB:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	default:
		break;
	}

	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = _InputLayout;
	gpipeline.InputLayout.NumElements = _InputLayoutCount;
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = _RTVFormat; //指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//テクスチャサンプラーの設定[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;

	//ルートシグネチャの生成
	rootSignatureDesc.Init_1_0(_RootParamCount, _RootParams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &m_ErrorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature[_PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = m_RootSignature[_PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_PipelineState[_PIPELINE_NUM]));
}

void PipelineManager::createInstancePipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC* inputLayout, ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//ブレンドステートの設定
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	//gpipeline.BlendState.AlphaToCoverageEnable = false;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutCount;//_countof(nputLayout)
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	/*if (PIPELINE_NUM == FBXSHADER_INS_GUIDE) {
		gpipeline.BlendState.AlphaToCoverageEnable = false;
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}*/

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[7] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(6, 0, D3D12_SHADER_VISIBILITY_ALL);//ビューとか
	rootparams[6].InitAsConstantBufferView(7, 0, D3D12_SHADER_VISIBILITY_ALL);//ビューとか

	//テクスチャサンプラーの設定[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;



	//ルートシグネチャの生成
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &m_ErrorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = m_RootSignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_PipelineState[PIPELINE_NUM]));
}

void PipelineManager::createInstanceDepthPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC* inputLayout, ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ブレンドステートの設定
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutCount;//_countof(nputLayout)
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[8] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
	rootparams[7].InitAsConstantBufferView(6, 0, D3D12_SHADER_VISIBILITY_ALL);//ビューとか

	//テクスチャサンプラーの設定[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;



	//ルートシグネチャの生成
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &m_ErrorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = m_RootSignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_PipelineState[PIPELINE_NUM]));
}

void PipelineManager::createInstanceShadowDepthPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC* inputLayout, ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ブレンドステートの設定
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutCount;//_countof(nputLayout)
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[8] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
	rootparams[7].InitAsConstantBufferView(6, 0, D3D12_SHADER_VISIBILITY_ALL);//ビューとか

	//テクスチャサンプラーの設定[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;



	//ルートシグネチャの生成
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &m_ErrorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = m_RootSignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_PipelineState[PIPELINE_NUM]));
}

void PipelineManager::createInstanceDOFPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC* inputLayout, ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ブレンドステートの設定
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutCount;//_countof(nputLayout)
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[8] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//
	rootparams[7].InitAsConstantBufferView(6, 0, D3D12_SHADER_VISIBILITY_ALL);//ビューとか

	//テクスチャサンプラーの設定[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;



	//ルートシグネチャの生成
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &m_ErrorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = m_RootSignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_PipelineState[PIPELINE_NUM]));
}
