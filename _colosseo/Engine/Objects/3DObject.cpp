#include "3DObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "..\..\BaseCollider.h"
#include "..\..\CollisionManager.h"
#include "Loader\FbxLoader.h"
#include "..\..\ShadowMapLight.h"
#include "..\..\DepthOfField.h"

#include "..\..\RenderManager.h"

My_F_List<Object3d> DirectX3dObject::object3ds;
My_F_List<InstanceObjectsData> DirectX3dObject::InstanceObject3ds;
int DirectX3dObject::OldShaderLoad = -1;
LightGroup *DirectX3dObject::lightGroup = nullptr;
bool DirectX3dObject::isUpdateOther = true;
bool DirectX3dObject::isUpdateOtherFlag = false;

int  DirectX3dObject::Counter = 0;
HANDLE DirectX3dObject::handle;

void DirectX3dObject::DirectX3DObjectReset(Window *Win) {
	OldShaderLoad = -1;
	TransConstBuffer();
}

void DirectX3dObject::CreateInstancePipiline(ShaderManager* _shader)
{
	//���_�V�F�[�_�[�ɓn�����߂̒��_�f�[�^�𐮂���
	D3D12_INPUT_ELEMENT_DESC InputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			offsetof(Vertex, pos),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			offsetof(Vertex, normal),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			offsetof(Vertex, uv),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"BONEINDICES", 0, DXGI_FORMAT_R16G16B16A16_SINT, 0,
			offsetof(Vertex, boneIndex),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"BONEINDICESB", 0, DXGI_FORMAT_R16G16B16A16_SINT, 0,
			offsetof(Vertex, boneIndexB),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			offsetof(Vertex, boneWeight),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		},
		{
			"BONEWEIGHTSB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			offsetof(Vertex, boneWeightB),
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
		}
	};
	PipelineManager::createInstancePipeline(FBXSHADER_INS, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEPS.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceDepthPipeline(Depth_SHEADER_INS, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Depth2Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceShadowDepthPipeline(Shadow_Depth_SHEADER_INS, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_VS_Inst.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_PS_Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceDOFPipeline(DOF_SHEADER_INS, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/DepthPSInst.hlsl", "main", "ps_5_0"));

	// Audience�p
	PipelineManager::createInstancePipeline(FBXSHADER_INS_AUD, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "AudMain", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEPS.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceDepthPipeline(Depth_SHEADER_INS_AUD, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "AudMain", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Depth2Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceShadowDepthPipeline(Shadow_Depth_SHEADER_INS_AUD, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_VS_Inst.hlsl", "AudMain", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_PS_Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceDOFPipeline(DOF_SHEADER_INS_AUD, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "AudMain", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/DepthPSInst.hlsl", "main", "ps_5_0"));

	// Guide�p
	PipelineManager::createInstancePipeline(FBXSHADER_INS_GUIDE, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEPS.hlsl", "Guide", "ps_5_0"));
	PipelineManager::createInstanceDepthPipeline(Depth_SHEADER_INS_GUIDE, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Depth2Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceShadowDepthPipeline(Shadow_Depth_SHEADER_INS_GUIDE, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_VS_Inst.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/Shadow_FBX_PS_Inst.hlsl", "main", "ps_5_0"));
	PipelineManager::createInstanceDOFPipeline(DOF_SHEADER_INS_GUIDE, _countof(InputLayout), InputLayout,
		_shader->GetShaderAndCompile(L"Resource/shader/FBXINSTANCEVS.hlsl", "main", "vs_5_0"),
		_shader->GetShaderAndCompile(L"Resource/shader/DepthPSInst.hlsl", "main", "ps_5_0"));
}

void DirectX3dObject::Draw() {
	/*for (int i = 0; i < (int)object3ds.size(); i++) {
		if (object3ds[i]->DrawFlag == true) {
			Drawobject3d(object3ds[i]);
		}
	}*/
}

void DirectX3dObject::TransConstBuffer() {

	for (int i = 0; i < (int)object3ds.size(); i++) {
		UpdateObject3d(object3ds[i]);
	}
}


DWORD __stdcall DirectX3dObject::T_UpdateObject3d(void* Vobject)
{
	Object3d* object = (Object3d*)Vobject;
	Do_UpdateObject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

DWORD __stdcall DirectX3dObject::T_UpdatetObject3dIns(void* Vobject)
{
	InstanceObjectsData* object = (InstanceObjectsData*)Vobject;
	Do_UpdateInstanceObject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

void DirectX3dObject::UpdateObject3d(Object3d* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 1) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		handle = CreateThread(NULL, 0, T_UpdateObject3d, object, 0, &dwID);
	}
	else {
		Do_UpdateObject3d(object);
	}
	if (DirectX3dObject::Counter == 0) {
		CloseHandle(handle);
	}
}

void DirectX3dObject::UpdateInstanceObject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 1) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		handle = CreateThread(NULL, 0, T_UpdatetObject3dIns, object, 0, &dwID);
	}
	else {
		Do_UpdateInstanceObject3d(object);
	}
	if (DirectX3dObject::Counter == 0) {
		CloseHandle(handle);
	}
}

Object3d *DirectX3dObject::CreateObject(Model *model, XMFLOAT3 pos, UINT ShaderNum)
{
	Object3d obj;
	obj.shaderNumber = ShaderNum;
	obj.position = { pos.x,pos.y,pos.z };
	obj.model = model;
	obj.scale = model->scale;

	obj.vertices = model->vertices;
	obj.indices = model->indices;
	obj.material = model->material;
	obj._indices = model->_indices; //���_��
	obj.isMaterial = model->isMaterial; //�}�e���A����K�����邩

	//obj.fbxScene == *model->GetFbxScene();

	object3ds.emplace_front(obj);
	//int count = (int)std::distance(object3ds.begin(), object3ds.end()) - 1;
	int count = 0;
	InitalizeObject3d(object3ds[count]);
	UpdateObject3d(object3ds[count]);

	int child = 0;
	for (int i = 0; i < ModelManager::Instance()->models.size(); i++) {
		if (ModelManager::Instance()->models[i].parent == model) {
			Object3d *a = CreateObject(&ModelManager::Instance()->models[i], pos, ShaderNum);
			a->parent = object3ds[count];
		}
	}

	return object3ds[count];
}

InstanceObjectsData* DirectX3dObject::CreateInstanceObject(Model* model, XMFLOAT3 pos, UINT ShaderNum, int InstanceCount, int MAX)
{
	InstanceObjectsData obj;
	obj.shaderNumber = ShaderNum;
	obj.model = model;
	obj.InstanceCount = InstanceCount;
	obj.INSTANCE_MAX = MAX;
	obj.object.resize(obj.INSTANCE_MAX);

	for (int i = 0; i < obj.INSTANCE_MAX; i++) {
		obj.object[i].position = pos;
		obj.object[i].scale = model->scale;
	}

	obj.vertices = model->vertices;
	obj.indices = model->indices;
	obj.material = model->material;
	obj._indices = model->_indices; //���_��
	obj.isMaterial = model->isMaterial; //�}�e���A����K�����邩

	//obj.fbxScene == *model->GetFbxScene();

	InstanceObject3ds.emplace_front(obj);
	//int count = (int)std::distance(object3ds.begin(), object3ds.end()) - 1;
	int count = 0;
	InitalizeInstanceObject3d(InstanceObject3ds[0]);
	UpdateInstanceObject3d(InstanceObject3ds[0]);

	return InstanceObject3ds[0];
}

void DirectX3dObject::AllObjectUpdate() {
	//Counter = 0;
	/*for (int i = 0; i < (int)object3ds.size(); i++) {
		Do_UpdateObject3d(object3ds[i]);
	};
	for (int i = 0; i < (int)InstanceObject3ds.size(); i++) {
		Do_UpdateInstanceObject3d(InstanceObject3ds[i]);
	}*/
	for (auto& data : object3ds) {
		Do_UpdateObject3d(&data);
	}
	for (auto& data : InstanceObject3ds) {
		Do_UpdateInstanceObject3d(&data);
	}
	//while (DirectX3dObject::Counter != 0) {
//
	//}
	isUpdateOtherFlag = false;
}

void DirectX3dObject::DeleteObject(Object3d *object) {
	for (int i = 0; i < (int)object3ds.size(); i++) {
		if (object == object3ds[i]) {
			//object3ds[i]->DeleteFunc();
			object3ds.erase_after(std::next(object3ds.before_begin(), i));
			break;
		}
	}
}

void DirectX3dObject::DeleteObject(InstanceObjectsData* object)
{
	for (int i = 0; i < (int)InstanceObject3ds.size(); i++) {
		if (object == InstanceObject3ds[i]) {
			//object3ds[i]->DeleteFunc();
			InstanceObject3ds.erase_after(std::next(InstanceObject3ds.before_begin(), i));
			break;
		}
	}
}

void DirectX3dObject::ShadowDraw()
{
	for (int i = 0; i < (int)object3ds.size(); i++) {
		Object3d* ptr = object3ds[i];
		if (ptr->UseShadow) { 
			ShadowDepthDrawobject3d(ptr); 
		}
	}
}

void DirectX3dObject::DepthDraw()
{
	for (int i = 0; i < (int)object3ds.size(); i++) {
		Object3d* ptr = object3ds[i];
		if (ptr->UseShadow) {
			DepthDrawobject3d(ptr);
		}
	}
}

void DirectX3dObject::BloomDraw()
{
	for (int i = 0; i < (int)object3ds.size(); i++) {
		Object3d* ptr = object3ds[i];
		if (ptr->UseBloom) {
			Drawobject3d(ptr);
		}
	}
}

void DirectX3dObject::BloomDepthDraw()
{
	for (int i = 0; i < (int)object3ds.size(); i++) {
		Object3d* ptr = object3ds[i];
		if (ptr->UseBloom) {
			DepthDrawobject3d(ptr);
		}
	}
}

void DirectX3dObject::DOFDraw()
{
	for (int i = 0; i < (int)object3ds.size(); i++) {
		Object3d* ptr = object3ds[i];
		if (ptr->UseDOF) {
			DOFDepthDrawobject3d(ptr);
		}
	}
}


void InitalizeObject3d(Object3d *object) {
	HRESULT result;

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * object->vertices.size());
	//���_�o�b�t�@����
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->vertBuff));
	//���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex *vertMap = nullptr;
	result = object->vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(object->vertices.begin(), object->vertices.end(), vertMap);
		object->vertBuff->Unmap(0, nullptr);
	}
	//���_�o�b�t�@�r���[(VBV)�̍쐬
	object->vbView.BufferLocation = object->vertBuff->GetGPUVirtualAddress();
	object->vbView.SizeInBytes = sizeVB;
	object->vbView.StrideInBytes = sizeof(object->vertices[0]);
	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * object->indices.size());
	//�C���f�b�N�X�o�b�t�@����
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->indexBuff));
	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = object->indexBuff->Map(0, nullptr, (void **)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(object->indices.begin(), object->indices.end(), indexMap);
		object->indexBuff->Unmap(0, nullptr);
	}
	//�C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	object->ibView.BufferLocation = object->indexBuff->GetGPUVirtualAddress();
	object->ibView.Format = DXGI_FORMAT_R16_UINT;
	object->ibView.SizeInBytes = sizeIB;

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffB0)
	);
	result = DirectXBase::dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffB00)
	);
	if (object->isMaterial) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object->material.constBuffB1));
	}

	//�萔�o�b�t�@�̐���(�X�L��)
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Object3d::ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffSkin)
	);

	//�萔�o�b�t�@�փf�[�^�]��
	Object3d::ConstBufferDataSkin *constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < Object3d::MAX_BONES; i++) {
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	object->constBuffSkin->Unmap(0, nullptr);

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataOther) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTime)
	);

	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataShadow) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffShadow)
	);

	//�f�X�N���v�^�P���̃T�C�Y
	UINT descHandleIncrementSize =
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	object->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	////CBV��CPU�f�X�N���v�^�n���h�����v�Z
	//object->cpuDescHandleCBV = descHeap->GetCPUDescriptorHandleForHeapStart();
	//object->cpuDescHandleCBV.ptr += index * descHandleIncrementSize;
	////CBV��GPU�f�X�N���v�^�n���h�����v�Z
	//object->gpuDescHandleCBV = descHeap->GetGPUDescriptorHandleForHeapStart();
	//object->gpuDescHandleCBV.ptr += index * descHandleIncrementSize;

	////�萔�o�b�t�@�r���[�̍쐬
	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	//cbvDesc.BufferLocation = object->constBuffB0->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = (UINT)object->constBuffB0->GetDesc().Width;
	//DirectXBase::dev->CreateConstantBufferView(&cbvDesc, object->cpuDescHandleCBV);
}

void InitalizeInstanceObject3d(InstanceObjectsData* object)
{
	HRESULT result;

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * object->vertices.size());
	//���_�o�b�t�@����
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->vertBuff));
	//���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	result = object->vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(object->vertices.begin(), object->vertices.end(), vertMap);
		object->vertBuff->Unmap(0, nullptr);
	}
	//���_�o�b�t�@�r���[(VBV)�̍쐬
	object->vbView.BufferLocation = object->vertBuff->GetGPUVirtualAddress();
	object->vbView.SizeInBytes = sizeVB;
	object->vbView.StrideInBytes = sizeof(object->vertices[0]);
	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * object->indices.size());
	//�C���f�b�N�X�o�b�t�@����
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->indexBuff));
	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = object->indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(object->indices.begin(), object->indices.end(), indexMap);
		object->indexBuff->Unmap(0, nullptr);
	}
	//�C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	object->ibView.BufferLocation = object->indexBuff->GetGPUVirtualAddress();
	object->ibView.Format = DXGI_FORMAT_R16_UINT;
	object->ibView.SizeInBytes = sizeIB;




	std::vector<ConstBufferDataB0Inst> instanceData(object->INSTANCE_MAX);
	{
		// �C���X�^���V���O���g����������ł���.
		for (int i = 0; i < object->INSTANCE_MAX; ++i)
		{
			XMMATRIX matScale, matRot, matTrans;
			//�X�P�[���A��]�A���s�ړ��s��̌v�Z
			matScale = XMMatrixScaling(object->object[i].scale.x, object->object[i].scale.y, object->object[i].scale.z);
			matRot = XMMatrixIdentity();
			matRot *= XMMatrixRotationZ(XMConvertToRadians(object->object[i].rotation.z));
			matRot *= XMMatrixRotationX(XMConvertToRadians(object->object[i].rotation.x));
			matRot *= XMMatrixRotationY(XMConvertToRadians(object->object[i].rotation.y));
			matTrans = XMMatrixTranslation(
				object->object[i].position.x, object->object[i].position.y, object->object[i].position.z
			);

			//���[���h�s��̍���
			object->object[i].matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
			object->object[i].matWorld *= matScale;
			object->object[i].matWorld *= matRot;
			object->object[i].matWorld *= matTrans;

			if (object->isBillboard) {

				object->object[i].matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
				object->object[i].matWorld *= matScale;
				object->object[i].matWorld *= matRot;
				object->object[i].matWorld *= Camera::matBillboard;
				object->object[i].matWorld *= matTrans;
			}

			auto& data = instanceData[i];
			data.world = object->object[i].matWorld;
			data.color = object->object[i].color;
			data.DrawFlag = true;
		}
	}


	//�萔�o�b�t�@�̃q�[�v�ݒ�
	 // �C���X�^���V���O�p�̃o�b�t�@������.
	UINT bufferSize = sizeof(ConstBufferDataB0Inst) * object->INSTANCE_MAX;
	bufferSize = RoundupConstantBufferSize(bufferSize);
	object->constBuffB0 = CreateBufferResource(
		D3D12_HEAP_TYPE_UPLOAD,
		bufferSize,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);

	void* mapped;
	object->constBuffB0->Map(0, nullptr, &mapped);
	memcpy(mapped, instanceData.data(), bufferSize);
	object->constBuffB0->Unmap(0, nullptr);


	if (object->isMaterial) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object->material.constBuffB1));
	}

	//�萔�o�b�t�@�̐���(�X�L��)
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Object3d::ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffSkin)
	);

	//�萔�o�b�t�@�փf�[�^�]��
	Object3d::ConstBufferDataSkin* constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < Object3d::MAX_BONES; i++) {
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	object->constBuffSkin->Unmap(0, nullptr);

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataOther) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTime)
	);

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataInst) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffB4)
	);

	//�萔�o�b�t�@�̐���
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataShadow) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffShadow)
	);

	//�f�X�N���v�^�P���̃T�C�Y
	UINT descHandleIncrementSize =
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	object->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void DirectX3dObject::Do_UpdateObject3d(Object3d *object) {
	XMMATRIX& matView = Camera::matView;
	XMMATRIX& matProjection = Camera::matProjection;
	if (object->isPlay) {
		//1 * Speed�t���[���i�߂�
		object->currentTime += object->frameTime * int(object->flameSpeed);
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (object->currentTime > object->endTime) {
			if (!object->isLoop && object->isEndStop == false) {
				object->currentTime = object->startTime;
				object->isPlay = false;
			}
			else {
				if (object->isEndStop == true) {
					object->isPlay = false;
				}
				else {
					object->currentTime = object->startTime;
				}
			}
		}
	}

	if (isUpdateOtherFlag == false) {
		if (object->oldposition != object->position || object->oldscale != object->scale || object->oldrotation != object->rotation ||
			Camera::eye != Camera::oldeye || Camera::target != Camera::oldtarget) {
			isUpdateOther = true;
			isUpdateOtherFlag = true;
		}
		else {
			isUpdateOther = false;
		}
	}

	HRESULT result;
	if (object->oldposition != object->position || object->oldscale != object->scale || object->oldrotation != object->rotation ||
		Camera::eye != Camera::oldeye || Camera::target != Camera::oldtarget || object->alwaysUpdate == true) {
		if (Camera::eye != Camera::oldeye) {
			Camera::oldeye = Camera::eye;
		}
		XMMATRIX matScale, matRot, matTrans;
		//�X�P�[���A��]�A���s�ړ��s��̌v�Z
		matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(object->rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(object->rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(object->rotation.y));
		matTrans = XMMatrixTranslation(
			object->position.x, object->position.y, object->position.z
		);

		//matBillboard = XMMatrixTranspose(matBillboard);

		//���[���h�s��̍���
		object->matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
		object->matWorld *= matScale;
		object->matWorld *= matRot;
		object->matWorld *= matTrans;

		object->matWorld2 = object->matWorld;

		if (object->isBillboard) {

			object->matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
			//object->matWorld = Camera::matBillboard;
			object->matWorld *= matScale;
			object->matWorld *= matRot;
			object->matWorld *= Camera::matBillboard;
			object->matWorld *= matTrans;
		}

		//�e�I�u�W�F�N�g�������
		if (object->parent != nullptr) {
			//�e�I�u�W�F�N�g�̃��[���h�s����|����
			object->matWorld *= object->parent->matWorld;
		}
	}

	//�萔�o�b�t�@�փf�[�^�]��
	if (object->model->isMaterial == false) {
		ConstBufferData *constMap = nullptr;
		if (SUCCEEDED(object->constBuffB0->Map(0, nullptr, (void **)&constMap))) {
			constMap->color = object->color; //RGBA
			constMap->mat = object->matWorld * matView * matProjection;
			object->constBuffB0->Unmap(0, nullptr);
		}
	}
	else {
		ConstBufferDataB0 *constMap0 = nullptr;
		if (SUCCEEDED(object->constBuffB0->Map(0, nullptr, (void **)&constMap0))) {
			//constMap0->mat = object->matWorld * matView * matProjection;
			constMap0->view = Camera::matView;
			constMap0->viewproj = Camera::matViewProjection;
			constMap0->viewproj2 = Camera::matViewProjection2;
			constMap0->world = object->matWorld;
			constMap0->cameraPos = Camera::eye;
			constMap0->color = object->color; //RGBA
			object->constBuffB0->Unmap(0, nullptr);
		}
	}


	if (object->model->isMaterial) {
		ConstBufferDataB1 *constMap1 = nullptr;
		result = object->material.constBuffB1->Map(0, nullptr, (void **)&constMap1);
		constMap1->ambient = object->material.ambient;
		constMap1->diffuse = object->material.diffuse;
		constMap1->specular = object->material.specular;
		constMap1->alpha = object->material.alpha;
		object->material.constBuffB1->Unmap(0, nullptr);
		//	}
	}
	object->oldposition = object->position;
	object->oldscale = object->scale;
	object->oldrotation = object->rotation;

	//�{�[���z��
	std::vector<Model::Bone> &bones = object->model->GetBones();
	//�萔�o�b�t�@�փf�[�^�]��
	Object3d::ConstBufferDataSkin *constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(object->currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	object->constBuffSkin->Unmap(0, nullptr);

	object->Timer = object->Timer + 1;
	ConstBufferDataOther *constMap2 = nullptr;
	result = object->constBuffTime->Map(0, nullptr, (void **)&constMap2);
	constMap2->time = object->Timer;
	constMap2->InterpSize = DepthOfField::InterpSize;
	constMap2->Focus = DepthOfField::Focus;
	constMap2->FocusSize = DepthOfField::FocusSize;
	constMap2->Flag = DepthOfField::UseFlag;
	object->constBuffTime->Unmap(0, nullptr);


	ConstBufferDataShadow *constMap3 = nullptr;
	if (SUCCEEDED(object->constBuffShadow->Map(0, nullptr, (void **)&constMap3))) {
		constMap3->Light_view = ShadowMapLight::matView;
		constMap3->Light_viewproj = ShadowMapLight::matViewProjection;
		constMap3->Light_Pos = ShadowMapLight::Pos;
		object->constBuffShadow->Unmap(0, nullptr);
	}
}

void DirectX3dObject::Do_UpdateInstanceObject3d(InstanceObjectsData* object)
{
	XMMATRIX& matView = Camera::matView;
	XMMATRIX& matProjection = Camera::matProjection;
	if (object->isPlay) {
		//1 * Speed�t���[���i�߂�
		object->currentTime += object->frameTime * int(object->flameSpeed);
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (object->currentTime > object->endTime) {
			if (!object->isLoop && object->isEndStop == false) {
				object->currentTime = object->startTime;
				object->isPlay = false;
			}
			else {
				if (object->isEndStop == true) {
					object->isPlay = false;
				}
				else {
					object->currentTime = object->startTime;
				}
			}
		}
	}

	// �C���X�^���V���O���g����������ł���.
	std::vector<ConstBufferDataB0Inst> instanceData(object->INSTANCE_MAX);
	for (int i = 0; i < object->INSTANCE_MAX; ++i)
	{
		if (isUpdateOtherFlag == false) {
			if (object->object[i].oldposition != object->object[i].position ||
				object->object[i].oldscale != object->object[i].scale ||
				object->object[i].oldrotation != object->object[i].rotation ||
				Camera::eye != Camera::oldeye || Camera::target != Camera::oldtarget) {
				isUpdateOther = true;
				isUpdateOtherFlag = true;
			}
			else {
				isUpdateOther = false;
			}
		}

		XMMATRIX matScale, matRot, matTrans;
		//�X�P�[���A��]�A���s�ړ��s��̌v�Z
		matScale = XMMatrixScaling(object->object[i].scale.x, object->object[i].scale.y, object->object[i].scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(object->object[i].rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(object->object[i].rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(object->object[i].rotation.y));
		matTrans = XMMatrixTranslation(
			object->object[i].position.x, object->object[i].position.y, object->object[i].position.z
		);

		//���[���h�s��̍���
		object->object[i].matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
		object->object[i].matWorld *= matScale;
		object->object[i].matWorld *= matRot;
		object->object[i].matWorld *= matTrans;

		if (object->isBillboard) {

			object->object[i].matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
			object->object[i].matWorld *= matScale;
			object->object[i].matWorld *= matRot;
			object->object[i].matWorld *= Camera::matBillboard;
			object->object[i].matWorld *= matTrans;
		}

		object->object[i].oldposition = object->object[i].position;
		object->object[i].oldscale = object->object[i].scale;
		object->object[i].oldrotation = object->object[i].rotation;

		auto& data = instanceData[i];
		data.world = object->object[i].matWorld;
		data.color = object->object[i].color;
		data.uv = XMFLOAT4(object->object[i].LT_UV.x, object->object[i].LT_UV.y, object->object[i].RB_UV.x, object->object[i].RB_UV.y);
		data.DrawFlag = object->object[i].isDraw;
	}

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	 // �C���X�^���V���O�p�̃o�b�t�@������.
	UINT bufferSize = sizeof(ConstBufferDataB0Inst) * object->INSTANCE_MAX;
	bufferSize = RoundupConstantBufferSize(bufferSize);
	/*object->constBuffB0 = CreateBufferResource(
		D3D12_HEAP_TYPE_UPLOAD,
		bufferSize,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);*/

	//�萔�o�b�t�@�փf�[�^�]��
	void* mapped;
	object->constBuffB0->Map(0, nullptr, &mapped);
	memcpy(mapped, instanceData.data(), bufferSize);
	object->constBuffB0->Unmap(0, nullptr);

	HRESULT result;



	if (object->model->isMaterial) {
		ConstBufferDataB1* constMap1 = nullptr;
		result = object->material.constBuffB1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = object->material.ambient;
		constMap1->diffuse = object->material.diffuse;
		constMap1->specular = object->material.specular;
		constMap1->alpha = object->material.alpha;
		object->material.constBuffB1->Unmap(0, nullptr);
		//	}
	}

	//�{�[���z��
	std::vector<Model::Bone>& bones = object->model->GetBones();
	//�萔�o�b�t�@�փf�[�^�]��
	Object3d::ConstBufferDataSkin* constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(object->currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	object->constBuffSkin->Unmap(0, nullptr);

	object->Timer = object->Timer + 1;
	ConstBufferDataInst* constMap2 = nullptr;
	result = object->constBuffB4->Map(0, nullptr, (void**)&constMap2);
	constMap2->view = Camera::matView;
	constMap2->viewproj = Camera::matViewProjection;
	constMap2->viewproj2 = Camera::matViewProjection2;
	constMap2->cameraPos = Camera::eye;
	object->constBuffB4->Unmap(0, nullptr);

	object->Timer = object->Timer + 1;
	ConstBufferDataOther* constMapT = nullptr;
	result = object->constBuffTime->Map(0, nullptr, (void**)&constMapT);
	constMapT->time = object->Timer;
	constMapT->InterpSize = DepthOfField::InterpSize;
	constMapT->Focus = DepthOfField::Focus;
	constMapT->FocusSize = DepthOfField::FocusSize;
	constMapT->Flag = DepthOfField::UseFlag;
	object->constBuffTime->Unmap(0, nullptr);



	ConstBufferDataShadow* constMap3 = nullptr;
	if (SUCCEEDED(object->constBuffShadow->Map(0, nullptr, (void**)&constMap3))) {
		constMap3->Light_view = ShadowMapLight::matView;
		constMap3->Light_viewproj = ShadowMapLight::matViewProjection;
		constMap3->Light_Pos = ShadowMapLight::Pos;
		object->constBuffShadow->Unmap(0, nullptr);
	}
}

/*

void Drawobject3d(Object3d* object)
{
	if (object == nullptr)return;
	while (DirectX3dObject::Counter != 0) {

	}
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_Drawobject3d, object, 0, &dwID);
	}
	//else {
	//	Do_Drawobject3d(object);
	//}
}

void Drawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;
	while (DirectX3dObject::Counter != 0) {

	}
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_Drawobject3dIns, object, 0, &dwID);
	}
	//else {
	//	Do_Drawobject3dIns(object);
	//}
}



DWORD __stdcall T_Drawobject3d(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	Object3d* object = (Object3d*)Vobject;
	Do_Drawobject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

DWORD __stdcall T_Drawobject3dIns(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	InstanceObjectsData* object = (InstanceObjectsData*)Vobject;
	Do_Drawobject3dIns(object);
	DirectX3dObject::Counter--;
	return 1;
}


void DepthDrawobject3d(Object3d* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_DepthDrawobject3d, object, 0, &dwID);
	}
	else {
		Do_DepthDrawobject3d(object);
	}
}

void DepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_DepthDrawobject3dIns, object, 0, &dwID);
	}
	else {
		Do_DepthDrawobject3dIns(object);
	}
}

DWORD __stdcall T_DepthDrawobject3d(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	Object3d* object = (Object3d*)Vobject;
	Do_DepthDrawobject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

DWORD __stdcall T_DepthDrawobject3dIns(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	InstanceObjectsData* object = (InstanceObjectsData*)Vobject;
	Do_DepthDrawobject3dIns(object);
	DirectX3dObject::Counter--;
	return 1;
}


void ShadowDepthDrawobject3d(Object3d* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_ShadowDepthDrawobject3d, object, 0, &dwID);
	}
	else {
		Do_ShadowDepthDrawobject3d(object);
	}
}

void ShadowDepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_ShadowDepthDrawobject3dIns, object, 0, &dwID);
	}
	else {
		Do_ShadowDepthDrawobject3dIns(object);
	}
}

DWORD __stdcall T_ShadowDepthDrawobject3d(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	Object3d* object = (Object3d*)Vobject;
	Do_ShadowDepthDrawobject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

DWORD __stdcall T_ShadowDepthDrawobject3dIns(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	InstanceObjectsData* object = (InstanceObjectsData*)Vobject;
	Do_ShadowDepthDrawobject3dIns(object);
	DirectX3dObject::Counter--;
	return 1;
}


void DOFDepthDrawobject3d(Object3d* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_DOFDepthDrawobject3d, object, 0, &dwID);
	}
	else {
		Do_DOFDepthDrawobject3d(object);
	}
}

void DOFDepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;
	if (DirectX3dObject::Counter < 4) {
		static DWORD dwID;
		DirectX3dObject::Counter++;
		CreateThread(NULL, 0, T_DOFDepthDrawobject3dIns, object, 0, &dwID);
	}
	else {
		Do_DOFDepthDrawobject3dIns(object);
	}
}

DWORD __stdcall T_DOFDepthDrawobject3d(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	Object3d* object = (Object3d*)Vobject;
	Do_DOFDepthDrawobject3d(object);
	DirectX3dObject::Counter--;
	return 1;
}

DWORD __stdcall T_DOFDepthDrawobject3dIns(void* Vobject)
{
	if (Vobject == nullptr)return 1;
	InstanceObjectsData* object = (InstanceObjectsData*)Vobject;
	Do_DOFDepthDrawobject3dIns(object);
	DirectX3dObject::Counter--;
	return 1;
}

*/

void Drawobject3d(Object3d *object) {
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	if (object->shaderNumber == Sea_SHADER) {
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	}

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);

	/*for (int i = 0; i < (int)DirectX3dObject::object3ds.size(); i++) {
		if (DirectX3dObject::object3ds[i]->parent == object) {
			Drawobject3d(DirectX3dObject::object3ds[i]);
		}
	}*/
}

void DepthDrawobject3d(Object3d *object)
{
	if (object == nullptr)return;

	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(object->constBuffB00->Map(0, nullptr, (void**)&constMap0))) {
		//constMap0->mat = object->matWorld * matView * matProjection;
		constMap0->view = Camera::matView;
		constMap0->viewproj = Camera::matViewProjection;
		constMap0->viewproj2 = Camera::matViewProjection2;
		constMap0->world = object->matWorld2;
		constMap0->cameraPos = Camera::eye;
		constMap0->color = object->color; //RGBA
		object->constBuffB00->Unmap(0, nullptr);
	}

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[Depth_SHEADER].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[Depth_SHEADER].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB00->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	//if (object->shaderNumber == Sea_SHADER) {
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	//}
	//DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);

	/*for (int i = 0; i < (int)DirectX3dObject::object3ds.size(); i++) {
		if (DirectX3dObject::object3ds[i]->parent == object) {
			Drawobject3d(DirectX3dObject::object3ds[i]);
		}
	}*/
}

void ShadowDepthDrawobject3d(Object3d *object)
{
	if (object == nullptr)return;

	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(object->constBuffB00->Map(0, nullptr, (void**)&constMap0))) {
		//constMap0->mat = object->matWorld * matView * matProjection;
		constMap0->view = Camera::matView;
		constMap0->viewproj = Camera::matViewProjection;
		constMap0->viewproj2 = Camera::matViewProjection2;
		constMap0->world = object->matWorld2;
		constMap0->cameraPos = Camera::eye;
		constMap0->color = object->color; //RGBA
		object->constBuffB00->Unmap(0, nullptr);
	}

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[Shadow_Depth_SHEADER].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[Shadow_Depth_SHEADER].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB00->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);

	/*for (int i = 0; i < (int)DirectX3dObject::object3ds.size(); i++) {
		if (DirectX3dObject::object3ds[i]->parent == object) {
			Drawobject3d(DirectX3dObject::object3ds[i]);
		}
	}*/
}

void DOFDepthDrawobject3d(Object3d *object)
{
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[DOF_SHEADER].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[DOF_SHEADER].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	//DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);

	/*for (int i = 0; i < (int)DirectX3dObject::object3ds.size(); i++) {
		if (DirectX3dObject::object3ds[i]->parent == object) {
			Drawobject3d(DirectX3dObject::object3ds[i]);
		}
	}*/
}

void Drawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());


	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffB4->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffTime->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, object->InstanceCount, 0, 0, 0);
}

void DepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber + 1].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber + 1].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(7, object->constBuffB4->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, object->InstanceCount, 0, 0, 0);
}

void ShadowDepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber + 2].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber + 2].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(7, object->constBuffB4->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, object->InstanceCount, 0, 0, 0);
}

void DOFDepthDrawobject3d(InstanceObjectsData* object)
{
	if (object == nullptr)return;

	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber + 3].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber + 3].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(7, object->constBuffB4->GetGPUVirtualAddress());
	//DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ���C�g�̕`��
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, object->InstanceCount, 0, 0, 0);
}

void Drawobject3dOfDefaultLight(Object3d *object)
{
	DirectX::XMINT2 WinSize = GetNowRenderSize();
	float WIDTH = float(WinSize.x);
	float HEIGHT = float(WinSize.y);

	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WIDTH, HEIGHT));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(WIDTH), LONG(HEIGHT)));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber].Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber].Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//�V�F�[�_�[���\�[�X�r���[
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}


	// ���C�g�̕`��
	DirectX3dObject::lightGroup->DefaultDraw(DirectXBase::cmdList.Get(), 3);

	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);
}

void Object3d::PlayAnimation(int AnimationNumber, bool Loop) {
	if (AnimationNumber == -1) {
		AnimationNumber = PlayAnimationNumber;
	}
	FbxScene *fbxScene = model->GetFbxScene();
	//�Đ�����A�j���[�V�����擾(�����͂̏ꍇ��0��)
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>(AnimationNumber);
	if (animstack == nullptr) {
		return;
	}
	//�A�j���[�V�����̖��O�擾
	const char *animstackname = animstack->GetName();
	fbxScene->SetCurrentAnimationStack(animstack);
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo(animstackname);
	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
	isEndStop = false;
	//���[�v�t���O��ύX����(�������͂��Ȃ������ꍇ�� false )
	isLoop = Loop;
	//�Đ����̃A�j���[�V�����ԍ���ύX����
	PlayAnimationNumber = AnimationNumber;
}

void Object3d::StopAnimation() {
	//�Đ���~��Ԃɂ���
	isPlay = false;
}

void Object3d::StartAnimation() {
	//�Đ�����Ԃɂ���
	isPlay = true;
}

void Object3d::ResetAnimetion() {
	FbxScene *fbxScene = model->GetFbxScene();
	//�Đ����̃A�j���[�V�����擾(���Đ��̏ꍇ��0)
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>(PlayAnimationNumber);
	//�A�j���[�V�����̖��O�擾
	const char *animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo(animstackname);
	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ���~��Ԃɂ���
	isPlay = false;
	//���[�v�t���O�����Z�b�g����
	isLoop = false;
}

void Object3d::ChangeAnimationSpeed(float Speed) {
	if (Speed < 0) {
		flameSpeed = 0;
	}
	else {
		flameSpeed = Speed;
	}
}

Object3d::~Object3d() {
	if (parent != nullptr) {
		delete parent;
	}
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Object3d::SetCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	this->collider = collider;
	CollisionManager::GetInstance()->AddCollider(collider);
}

void Object3d::ColliderUpdate() {
	if (collider) {
		collider->Update();
	}
}

void Object3d::DeleteFunc()
{
	delete collider;
	delete ptr;
	delete model;
	delete parent;
}
