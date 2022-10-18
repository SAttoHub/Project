#include "Primitive3D.h"
#include "..\..\Camera.h"

void Primitive3D::SetupCubePrimitive()
{
	LoadShader(&Cube_vsBlob, L"Resource/shader/CubePrimitiveVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Cube_gsBlob, L"Resource/shader/CubePrimitiveGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Cube_psBlob, L"Resource/shader/CubePrimitivePixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC CubePrimitiveInputLayout[] = {
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
			"SCALE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"ROTATE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"LIGHTING", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Cube_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Cube_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Cube_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = CubePrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(CubePrimitiveInputLayout);//_countof(nputLayout)
	//�}�`�̌`���_�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//�f�X�N���v�^�e�[�u���̐ݒ�
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&CubeRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = CubeRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&CubePipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void Primitive3D::SetupLinePrimitive()
{
	LoadShader(&Line_vsBlob, L"Resource/shader/LinePrimitiveVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Line_gsBlob, L"Resource/shader/LinePrimitiveGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Line_psBlob, L"Resource/shader/LinePrimitivePixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC LinePrimitiveInputLayout[] = {
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
	};
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Line_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Line_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Line_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = LinePrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(LinePrimitiveInputLayout);//_countof(nputLayout)
	//�}�`�̌`���_�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&LineRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = LineRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&LinePipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void Primitive3D::SetupTetrahedronPrimitive()
{
	LoadShader(&Tetrahedron_vsBlob, L"Resource/shader/TetrahedronVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Tetrahedron_gsBlob, L"Resource/shader/TetrahedronGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Tetrahedron_psBlob, L"Resource/shader/TetrahedronPixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC TetrahedronPrimitiveInputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"POSITIONA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"POSITIONB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"POSITIONC", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"LIGHTING", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Tetrahedron_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Tetrahedron_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Tetrahedron_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = TetrahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(TetrahedronPrimitiveInputLayout);//_countof(nputLayout)
	//�}�`�̌`���_�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&TetrahedronRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = TetrahedronRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&TetrahedronPipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void Primitive3D::SetupOctahedronPrimitive()
{
	LoadShader(&Octahedron_vsBlob, L"Resource/shader/OctahedronVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Octahedron_gsBlob, L"Resource/shader/OctahedronGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Octahedron_psBlob, L"Resource/shader/OctahedronPixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC OctahedronPrimitiveInputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"LIGHTING", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Octahedron_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Octahedron_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Octahedron_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = OctahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(OctahedronPrimitiveInputLayout);//_countof(nputLayout)
	//�}�`�̌`���_�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&OctahedronRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = OctahedronRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&OctahedronPipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void Primitive3D::SetupIcosahedronPrimitive()
{
	LoadShader(&Icosahedron_vsBlob, L"Resource/shader/IcosahedronVertex.hlsl", "main", "vs_5_0");
	LoadShader(&Icosahedron_gsBlob, L"Resource/shader/IcosahedronGeometry.hlsl", "main", "gs_5_0");
	LoadShader(&Icosahedron_psBlob, L"Resource/shader/IcosahedronPixel.hlsl", "main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC IcosahedronPrimitiveInputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"LIGHTING", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W��ݒ肷��\����
	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(Icosahedron_vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(Icosahedron_gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(Icosahedron_psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = IcosahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(IcosahedronPrimitiveInputLayout);//_countof(nputLayout)
	//�}�`�̌`���_�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&IcosahedronRootsignature));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = IcosahedronRootsignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&IcosahedronPipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void Primitive3D::Initialize()
{
	//���_�o�b�t�@�̐���
	HRESULT result;
#pragma region ���v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(CubePrimitive3D::Cube) * MaxCubePrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&CubeVertBuff)
	);
	CubevbView.BufferLocation = CubeVertBuff->GetGPUVirtualAddress();
	CubevbView.SizeInBytes = sizeof(CubePrimitive3D::Cube) * MaxCubePrimitives;
	CubevbView.StrideInBytes = sizeof(CubePrimitive3D::Cube);
#pragma endregion
#pragma region ���v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(LinePrimitive3D::Line) * MaxLinePrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&LineVertBuff)
	);
	LinevbView.BufferLocation = LineVertBuff->GetGPUVirtualAddress();
	LinevbView.SizeInBytes = sizeof(LinePrimitive3D::Line) * MaxLinePrimitives;
	LinevbView.StrideInBytes = sizeof(LinePrimitive3D::Line);
#pragma endregion
#pragma region ���l�ʑ̃v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(TetrahedronPrimitive::Tetrahedron) * MaxTetrahedronPrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&TetrahedronVertBuff)
	);
	TetrahedronvbView.BufferLocation = TetrahedronVertBuff->GetGPUVirtualAddress();
	TetrahedronvbView.SizeInBytes = sizeof(TetrahedronPrimitive::Tetrahedron) * MaxTetrahedronPrimitives;
	TetrahedronvbView.StrideInBytes = sizeof(TetrahedronPrimitive::Tetrahedron);
#pragma endregion
#pragma region �����ʑ̃v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(OctahedronPrimitive::Octahedron) * MaxOctahedronPrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&OctahedronVertBuff)
	);
	OctahedronvbView.BufferLocation = OctahedronVertBuff->GetGPUVirtualAddress();
	OctahedronvbView.SizeInBytes = sizeof(OctahedronPrimitive::Octahedron) * MaxOctahedronPrimitives;
	OctahedronvbView.StrideInBytes = sizeof(OctahedronPrimitive::Octahedron);
#pragma endregion
#pragma region ����\�ʑ̃v���~�e�B�u
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(IcosahedronPrimitive3D::Icosahedron) * MaxIcosahedronPrimitives),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&IcosahedronVertBuff)
	);
	IcosahedronvbView.BufferLocation = IcosahedronVertBuff->GetGPUVirtualAddress();
	IcosahedronvbView.SizeInBytes = sizeof(IcosahedronPrimitive3D::Icosahedron) * MaxIcosahedronPrimitives;
	IcosahedronvbView.StrideInBytes = sizeof(IcosahedronPrimitive3D::Icosahedron);
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

	SetupCubePrimitive();
	SetupLinePrimitive();
	SetupTetrahedronPrimitive();
	SetupOctahedronPrimitive();
	SetupIcosahedronPrimitive();
}

void Primitive3D::Update()
{
	for (int i = 0; i < MaxCubePrimitives; i++) {
		CubeData[i].Data.pos1 = XMFLOAT3{ 0,0,0 };
		CubeData[i].Data.pos2 = XMFLOAT3{ 0,0,0 };
		CubeData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		CubeData[i].Data.Lighting = false; // ���C�e�B���O���L����
		CubeData[i].Active = false;
	}
	for (int i = 0; i < MaxLinePrimitives; i++) {
		LineData[i].Data.pos1 = XMFLOAT3{ 0,0,0 };
		LineData[i].Data.pos2 = XMFLOAT3{ 0,0,0 };
		LineData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		LineData[i].Active = false;
	}
	for (int i = 0; i < MaxOctahedronPrimitives; i++) {
		OctahedronData[i].Data.pos = XMFLOAT3{ 0,0,0 };
		OctahedronData[i].Data.Radius = 0;
		OctahedronData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		OctahedronData[i].Data.Lighting = false; // ���C�e�B���O���L����
		OctahedronData[i].Active = false;
	}
	for (int i = 0; i < MaxIcosahedronPrimitives; i++) {
		IcosahedronData[i].Data.pos = XMFLOAT3{ 0,0,0 };
		IcosahedronData[i].Data.Radius = 0;
		IcosahedronData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		IcosahedronData[i].Data.Lighting = false; // ���C�e�B���O���L����
		IcosahedronData[i].Active = false;
	}
	for (int i = 0; i < MaxTetrahedronPrimitives; i++) {
		TetrahedronData[i].Data.pos1 = XMFLOAT3{ 0,0,0 };
		TetrahedronData[i].Data.pos2 = XMFLOAT3{ 0,0,0 };
		TetrahedronData[i].Data.pos3 = XMFLOAT3{ 0,0,0 };
		TetrahedronData[i].Data.pos4 = XMFLOAT3{ 0,0,0 };
		TetrahedronData[i].Data.color = XMFLOAT4{ 0,0,0,0 }; // �v���~�e�B�u�̐F
		TetrahedronData[i].Data.Lighting = false; // ���C�e�B���O���L����
		TetrahedronData[i].Active = false;
	}
}

void Primitive3D::CubeDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxCubePrimitives; i++) {
		if (CubeData[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	CubePrimitive3D::Cube *vertMap = nullptr;
	result = CubeVertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxCubePrimitives; i++) {
			if (CubeData[i].Active) {
				vertMap->pos1 = CubeData[i].Data.pos1;
				vertMap->pos2 = CubeData[i].Data.pos2;
				vertMap->Scale = CubeData[i].Data.Scale;
				vertMap->Rotate = CubeData[i].Data.Rotate;
				vertMap->color = CubeData[i].Data.color;
				vertMap->Lighting = CubeData[i].Data.Lighting;
				vertMap++;
			}
		}
	}
	CubeVertBuff->Unmap(0, nullptr);

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(CubeRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(CubePipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &CubevbView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	//ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	//DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	/*DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		Tex,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));*/

	// ���C�g�̕`��
	//DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::LineDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxLinePrimitives; i++) {
		if (LineData[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	LinePrimitive3D::Line *vertMap = nullptr;
	result = LineVertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxLinePrimitives; i++) {
			if (LineData[i].Active) {
				vertMap->pos1 = LineData[i].Data.pos1;
				vertMap->pos2 = LineData[i].Data.pos2;
				vertMap->color = LineData[i].Data.color;
				vertMap++;
			}
		}
	}
	LineVertBuff->Unmap(0, nullptr);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(LineRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(LinePipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &LinevbView);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawTetrahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxTetrahedronPrimitives; i++) {
		if (TetrahedronData[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	TetrahedronPrimitive::Tetrahedron *vertMap = nullptr;
	result = TetrahedronVertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxTetrahedronPrimitives; i++) {
			if (TetrahedronData[i].Active) {
				vertMap->pos1 = TetrahedronData[i].Data.pos1;
				vertMap->pos2 = TetrahedronData[i].Data.pos2;
				vertMap->pos3 = TetrahedronData[i].Data.pos3;
				vertMap->pos4 = TetrahedronData[i].Data.pos4;
				vertMap->color = TetrahedronData[i].Data.color;
				vertMap->Lighting = TetrahedronData[i].Data.Lighting;
				vertMap++;
			}
		}
	}
	TetrahedronVertBuff->Unmap(0, nullptr);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(TetrahedronRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(TetrahedronPipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &TetrahedronvbView);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawOctahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxOctahedronPrimitives; i++) {
		if (OctahedronData[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	OctahedronPrimitive::Octahedron *vertMap = nullptr;
	result = OctahedronVertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxOctahedronPrimitives; i++) {
			if (OctahedronData[i].Active) {
				vertMap->pos = OctahedronData[i].Data.pos;
				vertMap->Radius = OctahedronData[i].Data.Radius;
				vertMap->color = OctahedronData[i].Data.color;
				vertMap->Lighting = OctahedronData[i].Data.Lighting;
				vertMap++;
			}
		}
	}
	OctahedronVertBuff->Unmap(0, nullptr);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(OctahedronRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(OctahedronPipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &OctahedronvbView);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawIcosahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < MaxIcosahedronPrimitives; i++) {
		if (IcosahedronData[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	IcosahedronPrimitive3D::Icosahedron *vertMap = nullptr;
	result = IcosahedronVertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (int i = 0; i < MaxIcosahedronPrimitives; i++) {
			if (IcosahedronData[i].Active) {
				vertMap->pos = IcosahedronData[i].Data.pos;
				vertMap->Radius = IcosahedronData[i].Data.Radius;
				vertMap->color = IcosahedronData[i].Data.color;
				vertMap->Lighting = IcosahedronData[i].Data.Lighting;
				vertMap++;
			}
		}
	}
	IcosahedronVertBuff->Unmap(0, nullptr);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//���[�g�V�O�l�`��
	DirectXBase::cmdList->SetGraphicsRootSignature(IcosahedronRootsignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(IcosahedronPipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &IcosahedronvbView);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::Draw()
{
	DrawTetrahedronAll();
	DrawOctahedronAll();
	DrawIcosahedronAll();
	CubeDrawAll();
	LineDrawAll();
}

void Primitive3D::DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting) {
	int Num = 0;
	for (int i = 0; i < MaxCubePrimitives; i++) {
		if (CubeData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxCubePrimitives);

	this->CubeData[Num].Data.pos1 = pos1;
	this->CubeData[Num].Data.pos2 = pos2;
	this->CubeData[Num].Data.Scale = scale;
	this->CubeData[Num].Data.Rotate = rotate;
	this->CubeData[Num].Data.color = color;
	this->CubeData[Num].Data.Lighting = Lighting;
	CubeData[Num].Active = true;
}

void Primitive3D::DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	int Num = 0;
	for (int i = 0; i < MaxLinePrimitives; i++) {
		if (LineData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxLinePrimitives);

	this->LineData[Num].Data.pos1 = pos1;
	this->LineData[Num].Data.pos2 = pos2;
	this->LineData[Num].Data.color = color;
	LineData[Num].Active = true;
}

void Primitive3D::DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting)
{
	int Num = 0;
	for (int i = 0; i < MaxTetrahedronPrimitives; i++) {
		if (TetrahedronData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxTetrahedronPrimitives);

	this->TetrahedronData[Num].Data.pos1 = pos1;
	this->TetrahedronData[Num].Data.pos2 = pos2;
	this->TetrahedronData[Num].Data.pos3 = pos3;
	this->TetrahedronData[Num].Data.pos4 = pos4;
	this->TetrahedronData[Num].Data.color = color;
	this->TetrahedronData[Num].Data.Lighting = Lighting;
	TetrahedronData[Num].Active = true;
}

void Primitive3D::DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting)
{
	int Num = 0;
	for (int i = 0; i < MaxOctahedronPrimitives; i++) {
		if (OctahedronData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxOctahedronPrimitives);

	this->OctahedronData[Num].Data.pos = pos;
	this->OctahedronData[Num].Data.Radius = Radius;
	this->OctahedronData[Num].Data.color = color;
	this->OctahedronData[Num].Data.Lighting = Lighting;
	OctahedronData[Num].Active = true;
}

void Primitive3D::DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting)
{
	int Num = 0;
	for (int i = 0; i < MaxIcosahedronPrimitives; i++) {
		if (IcosahedronData[i].Active == false) break;
		Num++;
	}
	assert(Num <= MaxIcosahedronPrimitives);

	this->IcosahedronData[Num].Data.pos = pos;
	this->IcosahedronData[Num].Data.Radius = Radius;
	this->IcosahedronData[Num].Data.color = color;
	this->IcosahedronData[Num].Data.Lighting = Lighting;
	IcosahedronData[Num].Active = true;
}

void Primitive3D::DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval,XMFLOAT4 color)
{
	float min = 1;
	float max = 1;
	float count = 0;
	int kei = 1;
	//X��
	{
		if (pos1.x < pos2.x) {
			min = pos1.x;
			max = pos2.x;
		}
		else {
			min = pos2.x;
			max = pos1.x;
		}
		while (max >= min) {
			DrawLine(XMFLOAT3{ min, pos1.y, pos1.z }, XMFLOAT3{ min, pos2.y, pos2.z }, color);
			min += interval;
		}
	}
	//Z��
	{
		if (pos1.z < pos2.z) {
			min = pos1.z;
			max = pos2.z;
		}
		else {
			min = pos2.z;
			max = pos1.z;
		}
		while (max >= min) {
			DrawLine(XMFLOAT3{ pos1.x, pos1.y, min }, XMFLOAT3{ pos2.x, pos2.y, min }, color);
			min += interval;
		}
	}
}

void Primitive3D::LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName) {
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
