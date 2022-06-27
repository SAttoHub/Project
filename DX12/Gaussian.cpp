#include "Gaussian.h"
#include "Camera.h"

void Gaussian::SetupGraphPrimitive()
{
	HRESULT result;
#pragma region �摜�t�����v���~�e�B�u
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
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

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ConstBuff0));

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	//�A�b�v���[�h�\
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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Graph_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Graph_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Graph_psBlob);
	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //�W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = GraphPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(GraphPrimitiveInputLayout);
	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.DepthStencilState.DepthEnable = false;
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	////�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //�e�N�X�`���p
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	//�e�N�X�`���T���v���[�̐ݒ�[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//���[�g�V�O�l�`���̐���
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//���[�g�V�O�l�`���̐���
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = DirectXBase::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&GraphRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = GraphRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&GraphPipelinestate));
}

void Gaussian::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
{
	HRESULT result;
	//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		FileName, //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		EntryPointName, ModelName, //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0,
		*(&blob), &errorBlob
	);

	//�V�F�[�_�[�̃G���[���e��\��
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void Gaussian::Initialize(int GaussNumber, float Power) {
	GaussNum = GaussNumber;
	Dispersion = Power;
	SetupGraphPrimitive();

	HRESULT result;
	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WINDOW_WIDTH,
		(UINT)WINDOW_HEIGHT,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	//�e�N�X�`���o�b�t�@�̐���
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
	{//�e�N�X�`���𔒃N���A
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
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	DirectXBase::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;
	//�V�F�[�_�[���\�[�X�r���[�쐬
	DirectXBase::dev->CreateShaderResourceView(TextureBuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v����
	result = DirectXBase::dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//�f�X�N���v�^�q�[�v��RTV�쐬
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DirectXBase::dev->CreateRenderTargetView(TextureBuff.Get(),
		&rtvDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());
	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	//�[�x�o�b�t�@�̐���
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
	//	D3D12_HEAP_FLAG_NONE,
	//	&depthResDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//	&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
	//	IID_PPV_ARGS(&depthBuff));
	//assert(SUCCEEDED(result));
	////DSV�p�f�X�N���v�^�q�[�v�ݒ�
	//D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	//DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DescHeapDesc.NumDescriptors = 1;
	////DSV�p�f�X�N���v�^�q�[�v���쐬
	//result = DirectXBase::dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	//assert(SUCCEEDED(result));
	////�f�X�N���v�^�q�[�v��DSV�쐬
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

	//���㒸�_���E�����_���Z�o
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
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		ConstBuff0->Unmap(0, nullptr);
	}

	//�K�E�V�A��
	const UINT Division = 8;

	ConstBufferDataGaussian *constMap1 = nullptr;
	if (SUCCEEDED(ConstBuffGaussian->Map(0, nullptr, (void **)&constMap1))) {
		//constMap1->time = Timer;
		float total = 0.0f;

		// �K�E�X�֐��ɂ��d�݂̌v�Z
		for (int i = 0; i < Division; i++)
		{
			float pos = (float)i * 2.0f;
			weight[i] = expf(-pos * pos * Dispersion);
			total += weight[i];
		}

		// �d�݂̋K�i��
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

	//�摜�̕`��
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
		//�r���[�|�[�g�̐ݒ�R�}���h
		DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
		//�V�U�[��`�̐ݒ�R�}���h
		DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));
		//���[�g�V�O�l�`��
		DirectXBase::cmdList->SetGraphicsRootSignature(GraphRootsignature.Get());
		//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
		DirectXBase::cmdList->SetPipelineState(GraphPipelinestate.Get());
		DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		//���_�o�b�t�@�̐ݒ�
		DirectXBase::cmdList->IASetVertexBuffers(0, 1, &GraphvbView[i]);
		//�f�X�N���v�^�q�[�v���Z�b�g
		//ID3D12DescriptorHeap *ppHeaps[] = { descHeapSRV.Get() };
		DirectXBase::cmdList->SetDescriptorHeaps(1, &Descriptor);
		//�萔�o�b�t�@�r���[���Z�b�g
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
		//�V�F�[�_�[���\�[�X�r���[
		//DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, Descriptor->GetGPUDescriptorHandleForHeapStart());
		
		//�萔�o�b�t�@�r���[���Z�b�g
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(2, ConstBuffGaussian->GetGPUVirtualAddress());
		//�`��R�}���h
		DirectXBase::cmdList->DrawInstanced(1, 1, 0, 0);
	}
}

void Gaussian::ClearDepth()
{
}

void Gaussian::PreDrawScene(int Num)
{
	//���\�[�X�o���A��ύX
	DirectXBase::cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(TextureBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	auto rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//for (int i = 0; i < Num; i++) {
	//	rtvH.ptr += DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//}
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	//DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, &DirectXBase::dsvHeap->GetCPUDescriptorHandleForHeapStart());
	DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//�r���[�|�[�g�̐ݒ�
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WINDOW_WIDTH, WINDOW_HEIGHT));
	//�V�U�����O��`�̐ݒ�
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	//�S��ʃN���A
	DirectXBase::cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	DirectXBase::cmdList->ClearDepthStencilView(DirectXBase::dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Gaussian::PostDrawScene()
{
	DirectXBase::cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(TextureBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
