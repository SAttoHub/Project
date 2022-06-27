#include "Gaussian.h"
#include "Camera.h"

void Gaussian::SetupGraphPrimitive()
{
	HRESULT result;
#pragma region 画像付き箱プリミティブ
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(PostGraphPrimitive::Graph) * MaxGraphPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&GraphVertBuff[i])
		);
		GraphvbView[i].BufferLocation = GraphVertBuff[i]->GetGPUVirtualAddress();
		GraphvbView[i].SizeInBytes = sizeof(PostGraphPrimitive::Graph) * MaxGraphPrimitives;
		GraphvbView[i].StrideInBytes = sizeof(PostGraphPrimitive::Graph);
	}
#pragma endregion

	//定数バッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ConstBuff0));

	//定数バッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataGaussian) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ConstBuffGaussian));

	LoadShader(&Graph_vsBlob, L"Resource/shader/PostEffectVS.hlsl", "main", "vs_5_0");
	if (GaussNum == 0) {
		LoadShader(&Graph_gsBlob, L"Resource/shader/GaussianXGS.hlsl", "main", "gs_5_0");
		LoadShader(&Graph_psBlob, L"Resource/shader/GaussianXPS.hlsl", "main", "ps_5_0");
	}
	else {
		LoadShader(&Graph_gsBlob, L"Resource/shader/GaussianYGS.hlsl", "main", "gs_5_0");
		LoadShader(&Graph_psBlob, L"Resource/shader/GaussianYPS.hlsl", "main", "ps_5_0");
	}

	D3D12_INPUT_ELEMENT_DESC GraphPrimitiveInputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"FLAG", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Graph_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Graph_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Graph_psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
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
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = GraphPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(GraphPrimitiveInputLayout);
	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.DepthStencilState.DepthEnable = false;
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	////デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&GraphRootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = GraphRootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&GraphPipelinestate));
}

void Gaussian::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	HRESULT result;
	//シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		FileName, //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		EntryPointName, ModelName, //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0,
		*(&blob), &errorBlob
	);

	//シェーダーのエラー内容を表示
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void Gaussian::Initialize(int GaussNumber, float Power) {
	GaussNum = GaussNumber;
	Dispersion = Power;
	SetupGraphPrimitive();

	HRESULT result;
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WINDOW_WIDTH,
		(UINT)WINDOW_HEIGHT,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	//テクスチャバッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
		IID_PPV_ARGS(&TextureBuff)
	);
	assert(SUCCEEDED(result));
	{//テクスチャを白クリア
		const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
		const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
		const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
		UINT *img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) {
			img[i] = 0xffffffff;
		}
		result = TextureBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	DirectXBase::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープ生成
	result = DirectXBase::dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//デスクリプタヒープにRTV作成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DirectXBase::dev->CreateRenderTargetView(TextureBuff.Get(),
		&rtvDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());
	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	//深度バッファの生成
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
	//	D3D12_HEAP_FLAG_NONE,
	//	&depthResDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//	&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
	//	IID_PPV_ARGS(&depthBuff));
	//assert(SUCCEEDED(result));
	////DSV用デスクリプタヒープ設定
	//D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	//DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DescHeapDesc.NumDescriptors = 1;
	////DSV用デスクリプタヒープを作成
	//result = DirectXBase::dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	//assert(SUCCEEDED(result));
	////デスクリプタヒープにDSV作成
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DirectXBase::dev->CreateDepthStencilView(depthBuff.Get(),
	//	&dsvDesc,
	//	descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void Gaussian::Draw(ID3D12DescriptorHeap *Descriptor)
{
	int Num = 0;
	assert(Num <= MaxGraphPrimitives);

	XMFLOAT2 pos1 = { 0,0 };
	XMFLOAT2 pos2 = { WINDOW_WIDTH,WINDOW_HEIGHT };

	//左上頂点を右下頂点を算出
	float x1 = pos1.x;
	float x2 = pos2.x;
	if (x1 < x2) {
		x1 = pos2.x;
		x2 = pos1.x;
	}
	float y1 = pos1.y;
	float y2 = pos2.y;
	if (y1 < y2) {
		y1 = pos2.y;
		y2 = pos1.y;
	}
	XMFLOAT3 p1 = { x1, y1, 0 };
	XMFLOAT3 p2 = { x2, y2, 0 };

	this->GraphData[Num].Data.pos1 = p1;
	this->GraphData[Num].Data.pos2 = p2;
	this->GraphData[Num].Data.color = XMFLOAT4{ 1,1,1,1 };
	this->GraphData[Num].Data.Draw3D = false;


	HRESULT result;
	int activeCount = 1;
	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		ConstBuff0->Unmap(0, nullptr);
	}

	//ガウシアン
	const UINT Division = 8;

	ConstBufferDataGaussian *constMap1 = nullptr;
	if (SUCCEEDED(ConstBuffGaussian->Map(0, nullptr, (void **)&constMap1))) {
		//constMap1->time = Timer;
		float total = 0.0f;

		// ガウス関数による重みの計算
		for (int i = 0; i < Division; i++)
		{
			float pos = (float)i * 2.0f;
			weight[i] = expf(-pos * pos * Dispersion);
			total += weight[i];
		}

		// 重みの規格化
		for (int i = 0; i < Division; i++) {
			weight[i] = weight[i] / total * 0.5f;
		}

		constMap1->Weight0 = weight[0];
		constMap1->Weight1 = weight[1];
		constMap1->Weight2 = weight[2];
		constMap1->Weight3 = weight[3];
		constMap1->Weight4 = weight[4];
		constMap1->Weight5 = weight[5];
		constMap1->Weight6 = weight[6];
		constMap1->Weight7 = weight[7];

		constMap1->Width = (float)WINDOW_WIDTH;
		constMap1->Height = (float)WINDOW_HEIGHT;
		constMap1->OffsetX = 16.0f / (float)WINDOW_WIDTH;
		constMap1->OffsetY = 16.0f / (float)WINDOW_HEIGHT;


		ConstBuffGaussian->Unmap(0, nullptr);
	}

	//

	//画像の描画
	for (int i = 0; i < activeCount; i++) {
		PostGraphPrimitive::Graph *vertMap = nullptr;
		result = GraphVertBuff[i]->Map(0, nullptr, (void **)&vertMap);
		XMFLOAT3 pos1, pos2;
		if (SUCCEEDED(result)) {
			if (!GraphData[i].Data.Draw3D) {
				pos1 = XMFLOAT3{ GraphData[i].Data.pos1.x / DirectXBase::Win_Width * 2 , GraphData[i].Data.pos1.y / DirectXBase::Win_Height * 2, 0.0f };
				pos2 = XMFLOAT3{ GraphData[i].Data.pos2.x / DirectXBase::Win_Width * 2, GraphData[i].Data.pos2.y / DirectXBase::Win_Height * 2, 0.0f };
			}
			else {
				pos1 = GraphData[i].Data.pos1;
				pos2 = GraphData[i].Data.pos2;
			}

			vertMap->pos1 = pos1;
			vertMap->pos2 = pos2;
			vertMap->color = GraphData[i].Data.color;
			vertMap->Draw3D = GraphData[i].Data.Draw3D;
			vertMap++;
		}
		GraphVertBuff[i]->Unmap(0, nullptr);
		//ビューポートの設定コマンド
		DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
		//シザー矩形の設定コマンド
		DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));
		//ルートシグネチャ
		DirectXBase::cmdList->SetGraphicsRootSignature(GraphRootsignature.Get());
		//パイプラインステートの設定コマンド
		DirectXBase::cmdList->SetPipelineState(GraphPipelinestate.Get());
		DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		//頂点バッファの設定
		DirectXBase::cmdList->IASetVertexBuffers(0, 1, &GraphvbView[i]);
		//デスクリプタヒープをセット
		//ID3D12DescriptorHeap *ppHeaps[] = { descHeapSRV.Get() };
		DirectXBase::cmdList->SetDescriptorHeaps(1, &Descriptor);
		//定数バッファビューをセット
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
		//シェーダーリソースビュー
		//DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, Descriptor->GetGPUDescriptorHandleForHeapStart());
		
		//定数バッファビューをセット
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(2, ConstBuffGaussian->GetGPUVirtualAddress());
		//描画コマンド
		DirectXBase::cmdList->DrawInstanced(1, 1, 0, 0);
	}
}

void Gaussian::ClearDepth()
{
}

void Gaussian::PreDrawScene(int Num)
{
	//リソースバリアを変更
	DirectXBase::cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(TextureBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	auto rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//for (int i = 0; i < Num; i++) {
	//	rtvH.ptr += DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//}
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	//DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, &DirectXBase::dsvHeap->GetCPUDescriptorHandleForHeapStart());
	DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//ビューポートの設定
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WINDOW_WIDTH, WINDOW_HEIGHT));
	//シザリング矩形の設定
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	//全画面クリア
	DirectXBase::cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	DirectXBase::cmdList->ClearDepthStencilView(DirectXBase::dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Gaussian::PostDrawScene()
{
	DirectXBase::cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(TextureBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
