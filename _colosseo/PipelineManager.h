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
	ShaderManager *Shader;//�V�F�[�_�[

	static int m_NowSetPipeline;

	static int PIPELINE_NUM;
	static ComPtr<ID3DBlob> m_ErrorBlob; //�G���[�I�u�W�F�N�g

	//���[�g�V�O�l�`���̐���
	static ComPtr<ID3D12RootSignature> m_RootSignature[MAX_PIPELINE];
	//�p�C�v���C���X�e�[�g�̐���
	static ComPtr<ID3D12PipelineState> m_PipelineState[MAX_PIPELINE];
public:

	PipelineManager(ShaderManager *Shader);
	~PipelineManager() {}

	/// <summary>
	/// �p�C�v���C���̐���
	/// </summary>
	/// <param name="_PIPELINE_NUM"> �p�C�v���C���𐶐�����C���f�b�N�X�@</param>
	/// <param name="_BlendMode">�@�u�����h���[�h�@</param>
	/// <param name="_CullMode">�@�J�����O���[�h�@</param>
	/// <param name="_InputLayoutCount">�@�C���v�b�g���C�A�E�g�z��̗v�f��_countof(_InputLayout) </param>
	/// <param name="_InputLayout">�@�C���v�b�g���C�A�E�g�z��@</param>
	/// <param name="_RootParamCount">�@���[�g�p�����[�^�z��̗v�f��_countof(_RootParams)�@</param>
	/// <param name="_RootParams">�@���[�g�p�����[�^�z��@</param>
	/// <param name="_RTVFormat">�@�����_�[�^�[�Q�b�g�r���[�̃t�H�[�}�b�g�@</param>
	/// <param name="_vsBlob">�@���_�V�F�[�_�[�@</param>
	/// <param name="_psBlob">�@�s�N�Z���V�F�[�_�[�@</param>
	/// <param name="_gsBlob">�@�W�I���g���V�F�[�_�[�@</param>
	static void CreatePipeline(int _PIPELINE_NUM, BlendMode _BlendMode, D3D12_CULL_MODE _CullMode,
		int _InputLayoutCount, D3D12_INPUT_ELEMENT_DESC* _InputLayout,
		int _RootParamCount, CD3DX12_ROOT_PARAMETER* _RootParams,
		DXGI_FORMAT _RTVFormat,
		ID3DBlob* _vsBlob, ID3DBlob* _psBlob, ID3DBlob* _gsBlob = nullptr);

	/*----- �g�p���鎞�ɃR�[�h�����ɂ����Ȃ����̂ŕʂɕ����� -----*/

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
	// �C���f�b�N�X�̃��[�g�V�O�l�`�����擾
	static ID3D12RootSignature* GetRootSignature(int Index) {
		return m_RootSignature[Index].Get();
	}
	// �C���f�b�N�X�̃p�C�v���C���X�e�[�g���擾
	static ID3D12PipelineState* GetPipelineState(int Index) {
		return m_PipelineState[Index].Get();
	}
};