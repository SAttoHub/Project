#include "Primitive2D.h"
#include "Camera.h"
#include "TexManager.h"

void Primitive2D::SetupBoxPrimitive()
{
	LoadShader(&Box_vsBlob, L"Resource/shader/BoxPrimitiveVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Box_gsBlob, L"Resource/shader/BoxPrimitiveGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Box_psBlob, L"Resource/shader/BoxPrimitivePixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC BoxPrimitiveInputLayout[] = {
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
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Box_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Box_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Box_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = BoxPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(BoxPrimitiveInputLayout);
	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	////�f�X�N���v�^�e�[�u���̐ݒ�
	//CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //�萔�p
	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //�e�N�X�`���p
	//descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	//descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);



	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&BoxRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = BoxRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&BoxPipelinestate));
}

void Primitive2D::SetupGraphPrimitive()
{
	LoadShader(&Graph_vsBlob, L"Resource/shader/GraphPrimitiveVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Graph_gsBlob, L"Resource/shader/GraphPrimitiveGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Graph_psBlob, L"Resource/shader/GraphPrimitivePixel.hlsl", "main", "ps_5_0");

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
	HRESULT result;
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
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	////�f�X�N���v�^�e�[�u���̐ݒ�
	//CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //�萔�p
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //�e�N�X�`���p
	//descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);



	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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

void Primitive2D::Initialize()
{
	//���_�o�b�t�@�̐���
	HRESULT result;
#pragma region ���v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(BoxPrimitive::Box) * MaxBoxPrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&BoxVertBuff)
	);
	BoxvbView.BufferLocation = BoxVertBuff->GetGPUVirtualAddress();
	BoxvbView.SizeInBytes = sizeof(BoxPrimitive::Box) * MaxBoxPrimitives;
	BoxvbView.StrideInBytes = sizeof(BoxPrimitive::Box);
#pragma endregion
#pragma region �摜�t�����v���~�e�B�u
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(GraphPrimitive::Graph) * MaxGraphPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&GraphVertBuff[i])
		);
		GraphvbView[i].BufferLocation = GraphVertBuff[i]->GetGPUVirtualAddress();
		GraphvbView[i].SizeInBytes = sizeof(GraphPrimitive::Graph) * MaxGraphPrimitives;
		GraphvbView[i].StrideInBytes = sizeof(GraphPrimitive::Graph);
	}
#pragma endregion
#pragma region �摜�t�����v���~�e�B�u(��)
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(GraphPrimitive::Graph) * MaxGraphPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&BackGraphVertBuff[i])
		);
		BackGraphvbView[i].BufferLocation = BackGraphVertBuff[i]->GetGPUVirtualAddress();
		BackGraphvbView[i].SizeInBytes = sizeof(GraphPrimitive::Graph) * MaxGraphPrimitives;
		BackGraphvbView[i].StrideInBytes = sizeof(GraphPrimitive::Graph);
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


	if (FAILED(result)) {
		assert(0);
	}

	SetupBoxPrimitive();
	SetupGraphPrimitive();
}

void Primitive2D::Update()
{
	for (int i = 0; i < MaxBoxPrimitives; i++) {
		BoxData[i].Data.pos1 = XMFLOAT3{ 0,0,0 };
		BoxData[i].Data.pos2 = XMFLOAT3{ 0,0,0 };
		BoxData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		BoxData[i].Data.Draw3D = false;
		BoxData[i].Active = false;
	}
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		GraphData[i].Data.pos1 = XMFLOAT3{ 0,0,0 };
		GraphData[i].Data.pos2 = XMFLOAT3{ 0,0,0 };
		GraphData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		GraphData[i].Data.Draw3D = false;
		GraphData[i].Active = false;
		GraphData[i].TextureNumber = 0;
		GraphData[i].isBack = false;
	}
}

void Primitive2D::BoxDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxBoxPrimitives; i++) {
		if (BoxData[i].Active) {

			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	BoxPrimitive::Box *vertMap = nullptr;
	result = BoxVertBuff->Map(0, nullptr, (void **)&vertMap);
	XMFLOAT3 pos1, pos2;
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxBoxPrimitives; i++) {
			if (BoxData[i].Active) {
				if (!BoxData[i].Data.Draw3D) {
					pos1 = XMFLOAT3{ BoxData[i].Data.pos1.x / DirectXBase::Win_Width * 2 , BoxData[i].Data.pos1.y / DirectXBase::Win_Height * 2, 0.0f };
					pos2 = XMFLOAT3{ BoxData[i].Data.pos2.x / DirectXBase::Win_Width * 2, BoxData[i].Data.pos2.y / DirectXBase::Win_Height * 2, 0.0f };
				}
				else {
					pos1 = BoxData[i].Data.pos1;
					pos2 = BoxData[i].Data.pos2;
				}

				vertMap->pos1 = pos1;
				vertMap->pos2 = pos2;
				vertMap->color = BoxData[i].Data.color;
				vertMap->Draw3D = BoxData[i].Data.Draw3D;
				vertMap++;
			}
		}
	}
	BoxVertBuff->Unmap(0, nullptr);

	//XMVECTOR wol = XMVECTOR{ BoxData[0].Data.pos1.x, BoxData[0].Data.pos1.y, 0,0 };
	//XMMATRIX MatWorld = XMMatrixIdentity();
	//MatWorld *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	//MatWorld *= XMMatrixTranslationFromVector(wol);

	///*sprdata->spriteMatWorld = XMMatrixIdentity();
	//sprdata->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprdata->spriteRotation));
	//sprdata->spriteMatWorld *= XMMatrixTranslationFromVector(sprdata->spritePosition);*/



	XMMATRIX matP = XMMatrixOrthographicOffCenterLH(0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height, 0.0f, 0.0f, 1.0f);



	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(BoxRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(BoxPipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &BoxvbView);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive2D::GraphDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active && GraphData[i].isBack == false) {

			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�摜�̕`��
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active && GraphData[i].isBack == false) {
			GraphPrimitive::Graph *vertMap = nullptr;
			result = GraphVertBuff[i]->Map(0, nullptr, (void **)&vertMap);
			XMFLOAT3 pos1, pos2;
			if (SUCCEEDED(result)) {
				if (GraphData[i].Active) {
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
			ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
			DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			//�萔�o�b�t�@�r���[���Z�b�g
			DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
			//�V�F�[�_�[���\�[�X�r���[
			DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
				TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
				GraphData[i].TextureNumber,
				DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

			//�`��R�}���h
			DirectXBase::cmdList->DrawInstanced(1, 1, 0, 0);
		}
	}
}

void Primitive2D::BackGraphDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active && GraphData[i].isBack) {

			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�摜�̕`��
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active && GraphData[i].isBack == true) {
			GraphPrimitive::Graph *vertMap = nullptr;
			result = BackGraphVertBuff[i]->Map(0, nullptr, (void **)&vertMap);
			XMFLOAT3 pos1, pos2;
			if (SUCCEEDED(result)) {
				if (GraphData[i].Active) {
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
			}
			BackGraphVertBuff[i]->Unmap(0, nullptr);
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
			DirectXBase::cmdList->IASetVertexBuffers(0, 1, &BackGraphvbView[i]);
			//�f�X�N���v�^�q�[�v���Z�b�g
			ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
			DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			//�萔�o�b�t�@�r���[���Z�b�g
			DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
			//�V�F�[�_�[���\�[�X�r���[
			DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
				TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
				GraphData[i].TextureNumber,
				DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

			//�`��R�}���h
			DirectXBase::cmdList->DrawInstanced(1, 1, 0, 0);
		}
	}
}

void Primitive2D::Draw()
{
	BoxDrawAll();
	GraphDrawAll();
}

void Primitive2D::BackDraw()
{
	BackGraphDrawAll();
	DirectXBase::ClearDepthBuffer();
}

void Primitive2D::DrawBox(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color)
{
	int Num = 0;
	for (int i = 0; i < MaxBoxPrimitives; i++) {
		if (BoxData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxBoxPrimitives);

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

	this->BoxData[Num].Data.pos1 = p1;
	this->BoxData[Num].Data.pos2 = p2;
	this->BoxData[Num].Data.color = color;
	this->BoxData[Num].Data.Draw3D = false;
	BoxData[Num].Active = true;
}

void Primitive2D::DrawBox(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color, bool Draw3D)
{
	int Num = 0;
	for (int i = 0; i < MaxBoxPrimitives; i++) {
		if (BoxData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxBoxPrimitives);

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
	XMFLOAT3 p1 = { 0, 0, 0 };
	XMFLOAT3 p2 = { 0, 0, 0 };
	if (Draw3D) {
		float z1 = pos1.z;
		float z2 = pos2.z;
		p1 = { x1, y1, z1 };
		p2 = { x2, y2, z2 };
	}
	else {
		p1 = { x1, y1, 0 };
		p2 = { x2, y2, 0 };
	}

	this->BoxData[Num].Data.pos1 = p1;
	this->BoxData[Num].Data.pos2 = p2;
	this->BoxData[Num].Data.color = color;
	this->BoxData[Num].Data.Draw3D = Draw3D;
	BoxData[Num].Active = true;
}

void Primitive2D::DrawGraph(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color)
{
	int Num = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxGraphPrimitives);

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
	this->GraphData[Num].Data.color = color;
	this->GraphData[Num].Data.Draw3D = false;
	GraphData[Num].Active = true;
	GraphData[Num].TextureNumber = TextureNumber;
	GraphData[Num].isBack = false;
}

void Primitive2D::DrawGraph(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color, bool Draw3D)
{
	int Num = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxGraphPrimitives);

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
	XMFLOAT3 p1 = { 0, 0, 0 };
	XMFLOAT3 p2 = { 0, 0, 0 };
	if (Draw3D) {
		float z1 = pos1.z;
		float z2 = pos2.z;
		p1 = { x1, y1, z1 };
		p2 = { x2, y2, z2 };
	}
	else {
		p1 = { x1, y1, 0 };
		p2 = { x2, y2, 0 };
	}

	this->GraphData[Num].Data.pos1 = p1;
	this->GraphData[Num].Data.pos2 = p2;
	this->GraphData[Num].Data.color = color;
	this->GraphData[Num].Data.Draw3D = Draw3D;
	GraphData[Num].Active = true;
	GraphData[Num].TextureNumber = TextureNumber;
	GraphData[Num].isBack = false;
}

void Primitive2D::DrawGraphBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color)
{
	int Num = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxGraphPrimitives);

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
	this->GraphData[Num].Data.color = color;
	this->GraphData[Num].Data.Draw3D = false;
	GraphData[Num].Active = true;
	GraphData[Num].TextureNumber = TextureNumber;
	GraphData[Num].isBack = true;
}

void Primitive2D::DrawGraphBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color, bool Draw3D)
{
	int Num = 0;
	for (int i = 0; i < MaxGraphPrimitives; i++) {
		if (GraphData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxGraphPrimitives);

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
	XMFLOAT3 p1 = { 0, 0, 0 };
	XMFLOAT3 p2 = { 0, 0, 0 };
	if (Draw3D) {
		float z1 = pos1.z;
		float z2 = pos2.z;
		p1 = { x1, y1, z1 };
		p2 = { x2, y2, z2 };
	}
	else {
		p1 = { x1, y1, 0 };
		p2 = { x2, y2, 0 };
	}

	this->GraphData[Num].Data.pos1 = p1;
	this->GraphData[Num].Data.pos2 = p2;
	this->GraphData[Num].Data.color = color;
	this->GraphData[Num].Data.Draw3D = Draw3D;
	GraphData[Num].Active = true;
	GraphData[Num].TextureNumber = TextureNumber;
	GraphData[Num].isBack = true;
}

void Primitive2D::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName)
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
