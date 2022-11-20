#include "PipelineManager.h"
int PipelineManager::nowSetPipeline = -1;
ComPtr<ID3DBlob> PipelineManager::errorBlob;
ComPtr<ID3D12RootSignature> PipelineManager::rootsignature[MAX_PIPELINE];
ComPtr<ID3D12PipelineState> PipelineManager::pipelinestate[MAX_PIPELINE];

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

	/*
		ConstantColor, //単色
		ConstantColorNoRighting, //単色(ライティングなし)
		SpriteNormal, //スプライト用シェーダー
		ObjectNormal, //オブジェクト用シェーダー
		ObjectNoRighting //オブジェクト用シェーダー(ライティングなし)
	*/

	/*Shader->LoadShader("FBXVS", L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");
	Shader->LoadShader("FBXPS", L"Resource/shader/FBXPS.hlsl", "main", "ps_5_0");

	Shader->LoadShader("FBXBumpPS", L"Resource/shader/FBXBumpPS.hlsl", "main", "ps_5_0");
	Shader->LoadShader("FBXHeightPS", L"Resource/shader/FBXHeightPS.hlsl", "main", "ps_5_0");

	Shader->LoadShader("FBX_VS", L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");
	Shader->LoadShader("FBX_VS", L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");
	Shader->LoadShader("FBX_VS", L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");
	Shader->LoadShader("FBX_VS", L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0");*/
	
	createFBXPipeline(FBXSHADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"), 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXPS.hlsl", "main", "ps_5_0"));

	createFBXBumpPipeline(FBX_Bump_SHADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/FBXBumpPS.hlsl", "main", "ps_5_0"));

	createFBXheightPipeline(FBX_height_SHADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/FBXHeightPS.hlsl", "main", "ps_5_0"));

	createFBXLinePipeline(FBX_Line_SHADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/FBXLineGS.hlsl", "main", "gs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/FBXLinePS.hlsl", "main", "ps_5_0"));

	createFBXSeaPipeline(Sea_SHADER, _countof(InputLayout), InputLayout,
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/SeaPS.hlsl", "main", "ps_5_0"));

	createFBXDepthPipeline(Depth_SHEADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/Depth2.hlsl", "main", "ps_5_0"));

	createFBXShadowDepthPipeline(Shadow_Depth_SHEADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_VS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_PS.hlsl", "main", "ps_5_0"));

	createFBXDOFPipeline(DOF_SHEADER, _countof(InputLayout), InputLayout, 
		Shader->GetShaderAndCompile(L"Resource/shader/FBXVS.hlsl", "main", "vs_5_0"),
		Shader->GetShaderAndCompile(L"Resource/shader/DepthPS.hlsl", "main", "ps_5_0"));
}

void PipelineManager::createFBXPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	CD3DX12_ROOT_PARAMETER rootparams[5] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXBumpPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = false;

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXheightPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXOutLinePipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = false;
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
	CD3DX12_ROOT_PARAMETER rootparams[5] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXLinePipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *gsBlob, ID3DBlob *psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = false;

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
	CD3DX12_ROOT_PARAMETER rootparams[5] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXSeaPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	CD3DX12_ROOT_PARAMETER rootparams[6] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXDepthPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	CD3DX12_ROOT_PARAMETER rootparams[7] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXShadowDepthPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	CD3DX12_ROOT_PARAMETER rootparams[7] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createFBXDOFPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	CD3DX12_ROOT_PARAMETER rootparams[7] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);//時間
	rootparams[6].InitAsConstantBufferView(5, 0, D3D12_SHADER_VISIBILITY_ALL);//

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}




/*
void PipelineManager::createPipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob)
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
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	if (PIPELINE_NUM == ObjMaterial2) {
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}
	else {
		blenddesc.BlendEnable = false;
	}

	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	gpipeline.BlendState.AlphaToCoverageEnable = true;
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
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //定数用
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
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
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = false;


	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.IndependentBlendEnable = true;
	gpipeline.BlendState.AlphaToCoverageEnable = true;
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
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //定数用
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[5] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);//スキニング用


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createSpritePipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *psBlob) {
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	//D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	//gpipeline.BlendState.IndependentBlendEnable = true;
	//blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	//if (PIPELINE_NUM == SpriteNormal) {
	//	blenddesc.BlendEnable = true;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//}
	//else {
	//	blenddesc.BlendEnable = false;
	//}
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

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
	CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //定数用
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);



	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[4] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//ライト用

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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));
}

void PipelineManager::createParticlePipeline(int PIPELINE_NUM, UINT inputLayoutCount, D3D12_INPUT_ELEMENT_DESC *inputLayout, ID3DBlob *vsBlob, ID3DBlob *gsBlob, ID3DBlob *psBlob)
{
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	////ブレンドステートの設定
	gpipeline.BlendState.IndependentBlendEnable = true;
	//D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	//blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	//blenddesc.BlendEnable = true;
	//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	/*blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	/*blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlendAlpha = D3D12_BLEND_DEST_ALPHA;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.AlphaToCoverageEnable = true;





	//gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutCount;//_countof(nputLayout)
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //定数用
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


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
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature[PIPELINE_NUM]));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature[PIPELINE_NUM].Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate[PIPELINE_NUM]));

	if (FAILED(result)) {
		assert(0);
	}
}
*/