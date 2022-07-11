#include "3DObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "FbxLoader.h"
#include "ShadowMapLight.h"
#include "DepthOfField.h"
My_F_List<Object3d> DirectX3dObject::object3ds;
int DirectX3dObject::OldShaderLoad = -1;
LightGroup *DirectX3dObject::lightGroup = nullptr;

void DirectX3dObject::DirectX3DObjectReset(Window *Win) {
	OldShaderLoad = -1;
	TransConstBuffer();
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
		UpdateObject3d(object3ds[i], Camera::matView, Camera::matProjection);
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
	obj._indices = model->_indices; //頂点数
	obj.isMaterial = model->isMaterial; //マテリアルを適応するか

	//obj.fbxScene == *model->GetFbxScene();

	object3ds.emplace_front(obj);
	//int count = (int)std::distance(object3ds.begin(), object3ds.end()) - 1;
	int count = 0;
	InitalizeObject3d(object3ds[count], count);
	UpdateObject3d(object3ds[count], Camera::matView, Camera::matProjection);

	int child = 0;
	for (int i = 0; i < ModelManager::Instance()->models.size(); i++) {
		if (ModelManager::Instance()->models[i].parent == model) {
			Object3d *a = CreateObject(&ModelManager::Instance()->models[i], pos, ShaderNum);
			a->parent = object3ds[count];
		}
	}

	return object3ds[count];
}

void DirectX3dObject::AllObjectUpdate() {
	for (int i = 0; i < (int)object3ds.size(); i++) {
		UpdateObject3d(object3ds[i], Camera::matView, Camera::matProjection);
	};
}

void DirectX3dObject::DeleteObject(Object3d *object) {
	for (int i = 0; i < (int)object3ds.size(); i++) {
		if (object == object3ds[i]) {
			object3ds.erase_after(std::next(object3ds.before_begin(), i));
			break;
		}
	}
}


void InitalizeObject3d(Object3d *object, int index) {
	HRESULT result;

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * object->vertices.size());
	//頂点バッファ生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->vertBuff));
	//頂点バッファへのデータ転送
	Vertex *vertMap = nullptr;
	result = object->vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(object->vertices.begin(), object->vertices.end(), vertMap);
		object->vertBuff->Unmap(0, nullptr);
	}
	//頂点バッファビュー(VBV)の作成
	object->vbView.BufferLocation = object->vertBuff->GetGPUVirtualAddress();
	object->vbView.SizeInBytes = sizeVB;
	object->vbView.StrideInBytes = sizeof(object->vertices[0]);
	//頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * object->indices.size());
	//インデックスバッファ生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->indexBuff));
	//インデックスバッファへのデータ転送
	unsigned short *indexMap = nullptr;
	result = object->indexBuff->Map(0, nullptr, (void **)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(object->indices.begin(), object->indices.end(), indexMap);
		object->indexBuff->Unmap(0, nullptr);
	}
	//インデックスバッファビュー(IBV)の作成
	object->ibView.BufferLocation = object->indexBuff->GetGPUVirtualAddress();
	object->ibView.Format = DXGI_FORMAT_R16_UINT;
	object->ibView.SizeInBytes = sizeIB;

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffB0)
	);
	if (object->isMaterial) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object->material.constBuffB1));
	}

	//定数バッファの生成(スキン)
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Object3d::ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffSkin)
	);

	//定数バッファへデータ転送
	Object3d::ConstBufferDataSkin *constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < Object3d::MAX_BONES; i++) {
		//合成してスキニング行列に
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	object->constBuffSkin->Unmap(0, nullptr);

	//定数バッファの生成(スキン)
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataOther) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTime)
	);

	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataShadow) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffShadow)
	);

	//デスクリプタ１つ分のサイズ
	UINT descHandleIncrementSize =
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	object->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	////CBVのCPUデスクリプタハンドルを計算
	//object->cpuDescHandleCBV = descHeap->GetCPUDescriptorHandleForHeapStart();
	//object->cpuDescHandleCBV.ptr += index * descHandleIncrementSize;
	////CBVのGPUデスクリプタハンドルを計算
	//object->gpuDescHandleCBV = descHeap->GetGPUDescriptorHandleForHeapStart();
	//object->gpuDescHandleCBV.ptr += index * descHandleIncrementSize;

	////定数バッファビューの作成
	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	//cbvDesc.BufferLocation = object->constBuffB0->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = (UINT)object->constBuffB0->GetDesc().Width;
	//DirectXBase::dev->CreateConstantBufferView(&cbvDesc, object->cpuDescHandleCBV);
}

void DirectX3dObject::UpdateObject3d(Object3d *object, XMMATRIX &matView, XMMATRIX &matProjection) {
	if (object->isPlay) {
		//1 * Speedフレーム進める
		object->currentTime += object->frameTime * object->flameSpeed;
		//最後まで再生したら先頭に戻す
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

	HRESULT result;
	/*if (object->oldposition != object->position || object->oldscale != object->scale || object->oldrotation != object->rotation ||
		Camera::eye != Camera::oldeye || object->alwaysUpdate == true) {
		if (Camera::eye != Camera::oldeye) {
			Camera::oldeye = Camera::eye;
		}*/
	XMMATRIX matScale, matRot, matTrans;
	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(object->rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(object->rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(object->rotation.y));
	matTrans = XMMatrixTranslation(
		object->position.x, object->position.y, object->position.z
	);

	//matBillboard = XMMatrixTranspose(matBillboard);

	//ワールド行列の合成
	object->matWorld = XMMatrixIdentity(); //変形をリセット
	object->matWorld *= matScale;
	object->matWorld *= matRot;
	object->matWorld *= matTrans;

	if (object->isBillboard) {

		object->matWorld = XMMatrixIdentity(); //変形をリセット
		object->matWorld *= Camera::matBillboard;
		object->matWorld *= matScale;
		object->matWorld *= matRot;
		object->matWorld *= matTrans;
	}

	//親オブジェクトがあれば
	if (object->parent != nullptr) {
		//親オブジェクトのワールド行列を掛ける
		object->matWorld *= object->parent->matWorld;
	}

	//定数バッファへデータ転送
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

	//ボーン配列
	std::vector<Model::Bone> &bones = object->model->GetBones();
	//定数バッファへデータ転送
	Object3d::ConstBufferDataSkin *constMapSkin = nullptr;
	result = object->constBuffSkin->Map(0, nullptr, (void **)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {
		//今の姿勢行列
		XMMATRIX matCurrentPose;
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(object->currentTime);
		//XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//合成してスキニング行列に
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

void Drawobject3d(Object3d *object) {
	if (object == nullptr)return;

	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber].Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber].Get());

	//プリミティブ形状の設定コマンド
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//インデックスバッファの設定
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	if (object->shaderNumber == Sea_SHADER) {
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	}

	// ライトの描画
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//描画コマンド
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

	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[Depth_SHEADER].Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[Depth_SHEADER].Get());

	//プリミティブ形状の設定コマンド
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//インデックスバッファの設定
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	//if (object->shaderNumber == Sea_SHADER) {
		DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	//}
	//DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ライトの描画
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//描画コマンド
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

	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[Shadow_Depth_SHEADER].Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[Shadow_Depth_SHEADER].Get());

	//プリミティブ形状の設定コマンド
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//インデックスバッファの設定
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ライトの描画
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//描画コマンド
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

	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[DOF_SHEADER].Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[DOF_SHEADER].Get());

	//プリミティブ形状の設定コマンド
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//インデックスバッファの設定
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(5, object->constBuffTime->GetGPUVirtualAddress());
	//DirectXBase::cmdList->SetGraphicsRootConstantBufferView(6, object->constBuffShadow->GetGPUVirtualAddress());

	// ライトの描画
	DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);

	//描画コマンド
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);

	/*for (int i = 0; i < (int)DirectX3dObject::object3ds.size(); i++) {
		if (DirectX3dObject::object3ds[i]->parent == object) {
			Drawobject3d(DirectX3dObject::object3ds[i]);
		}
	}*/
}

void Drawobject3dOfDefaultLight(Object3d *object)
{
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, DirectXBase::Win_Width, DirectXBase::Win_Height));

	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[object->shaderNumber].Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[object->shaderNumber].Get());

	//プリミティブ形状の設定コマンド
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//インデックスバッファの設定
	DirectXBase::cmdList->IASetIndexBuffer(&object->ibView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(1, object->material.constBuffB1->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->material.texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(4, object->constBuffSkin->GetGPUVirtualAddress());

	if (object->shaderNumber == FBX_Bump_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	else if (object->shaderNumber == FBX_height_SHADER) {
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(5, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.BumpTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		//シェーダーリソースビュー
		DirectXBase::cmdList->SetGraphicsRootDescriptorTable(6, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			object->material.HeightTexNumber,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}


	// ライトの描画
	DirectX3dObject::lightGroup->DefaultDraw(DirectXBase::cmdList.Get(), 3);

	//描画コマンド
	DirectXBase::cmdList->DrawIndexedInstanced(object->_indices, 1, 0, 0, 0);
}

void Object3d::PlayAnimation(int AnimationNumber, bool Loop) {
	if (AnimationNumber == -1) {
		AnimationNumber = PlayAnimationNumber;
	}
	FbxScene *fbxScene = model->GetFbxScene();
	//再生するアニメーション取得(未入力の場合は0番)
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>(AnimationNumber);
	if (animstack == nullptr) {
		return;
	}
	//アニメーションの名前取得
	const char *animstackname = animstack->GetName();
	fbxScene->SetCurrentAnimationStack(animstack);
	//アニメーションの時間情報
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo(animstackname);
	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生中状態にする
	isPlay = true;
	isEndStop = false;
	//ループフラグを変更する(何も入力しなかった場合は false )
	isLoop = Loop;
	//再生中のアニメーション番号を変更する
	PlayAnimationNumber = AnimationNumber;
}

void Object3d::StopAnimation() {
	//再生停止状態にする
	isPlay = false;
}

void Object3d::StartAnimation() {
	//再生中状態にする
	isPlay = true;
}

void Object3d::ResetAnimetion() {
	FbxScene *fbxScene = model->GetFbxScene();
	//再生中のアニメーション取得(未再生の場合は0)
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>(PlayAnimationNumber);
	//アニメーションの名前取得
	const char *animstackname = animstack->GetName();
	//アニメーションの時間情報
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo(animstackname);
	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生停止状態にする
	isPlay = false;
	//ループフラグをリセットする
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
