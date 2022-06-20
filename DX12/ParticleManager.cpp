#include "ParticleManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "ParticleEmitter.h"

void ParticleManager::DirectX3DObjectReset(Window *Win) {
	OldShaderLoad = -1;
	CreateConstBufferView();
	TransConstBuffer();

	//HRESULT result;
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * MaxParticle * constantBufferNum),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff)
	//);

	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeof(ParticleVertexPos) * MaxParticle * constantBufferNum;
	//vbView.StrideInBytes = sizeof(ParticleVertexPos);


	////定数バッファのヒープ設定
	//D3D12_HEAP_PROPERTIES heapprop{};
	//heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	////定数バッファのリソース設定
	//D3D12_RESOURCE_DESC resdesc{};
	//resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
	//resdesc.Height = 1;
	//resdesc.DepthOrArraySize = 1;
	//resdesc.MipLevels = 1;
	//resdesc.SampleDesc.Count = 1;
	//resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	////定数バッファの生成
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&heapprop,
	//	D3D12_HEAP_FLAG_NONE,
	//	&resdesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB0)
	//);
}

void ParticleManager::Draw() {
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)DirectXBase::Win_Width, (LONG)DirectXBase::Win_Height));
	//ルートシグネチャの設定コマンド
	DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[Particle].Get());

	DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[Particle].Get());

 	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	UpdateObject3d(Camera::matView, Camera::matProjection);
	for (int i = 0; i < object3ds.size(); i++) {
		ParDrawobject3d(&object3ds[i]);
	}
}

void ParticleManager::SetBlend(int Blend_Num) {

}

void ParticleManager::TransConstBuffer() {


	//for (int i = 0; i < object3ds.size(); i++) {
		//UpdateObject3d(matView, matProjection);
	//}
}

void ParticleManager::CreateConstBufferView() {

}


void ParticleManager::Create(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, UINT texNum, UINT ShaderNum, XMFLOAT4 startColor, XMFLOAT4 endColor) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.velocity = vel;
	par.accel = acc;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;
	par.startColor = startColor;
	par.endColor = endColor;
	const float width = 10.0f;
	for (int i = 0; i < count; i++) {
		if (i == 0) {

		}
		else {
			par.position = XMFLOAT3{ (float)rand() / RAND_MAX * width - width / 2.0f + pos.x,
				(float)rand() / RAND_MAX * width - width / 2.0f + pos.y,
				(float)rand() / RAND_MAX * width - width / 2.0f + pos.z };
		}
	obj.State.emplace_back(par);
	}

	object3ds.push_back(obj);

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * object3ds[(int)object3ds.size() - 1].State.size() * object3ds[(int)object3ds.size() - 1].MaxParticle),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].vertBuff)
	);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[object3ds.size() - 1].State.size() * (UINT)object3ds[object3ds.size() - 1].MaxParticle;
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);
	//int count = object3ds.size() - 1;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}

void ParticleManager::CreateRand(std::string tag,
	int count,
	XMFLOAT3 pos,
	float Rand_width,
	XMFLOAT3 acc,
	int lifetime,
	float start_scale,
	float end_scale,
	UINT texNum,
	UINT ShaderNum,
	XMFLOAT4 startColor,
	XMFLOAT4 endColor) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.accel = acc;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;
	par.startColor = startColor;
	par.endColor = endColor;
	for (int i = 0; i < count; i++) {
		/*par.velocity = XMFLOAT3{ (float)rand() / RAND_MAX * Rand_width - Rand_width / 2.0f,
			(float)rand() / RAND_MAX * Rand_width - Rand_width / 2.0f,
			(float)rand() / RAND_MAX * Rand_width - Rand_width / 2.0f };*/
		par.velocity = XMFLOAT3{ (float)rand() / RAND_MAX * Rand_width - Rand_width / 2.0f,
			(float)rand() / RAND_MAX * Rand_width - Rand_width / 2.0f,
			0 };
		obj.State.emplace_back(par);
	}

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * obj.State.size()),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj.vertBuff)
	);

	object3ds.push_back(obj);


	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[object3ds.size() - 1].State.size();
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}

void ParticleManager::UpdateObject3d(XMMATRIX &matView, XMMATRIX &matProjection) {
	HRESULT result;
	float f = 0;
	for (int i = 0; i < object3ds.size(); i++) {
		//粒の更新
		int a = 0;

		for (int j = 0; j < object3ds[i].State.size(); j++) {
			if (object3ds[i].State[j].now_frame < object3ds[i].State[j].end_frame) {
				if (object3ds[i].State[j].Type == P_Normal) {
					f = (float)object3ds[i].State[j].end_frame / (float)object3ds[i].State[j].now_frame;
					object3ds[i].State[j].scale = (object3ds[i].State[j].end_scale - object3ds[i].State[j].start_scale) / f;
					object3ds[i].State[j].scale += object3ds[i].State[j].start_scale;

					object3ds[i].State[j].color.x = (object3ds[i].State[j].endColor.x - object3ds[i].State[j].startColor.x) / f;
					object3ds[i].State[j].color.y = (object3ds[i].State[j].endColor.y - object3ds[i].State[j].startColor.y) / f;
					object3ds[i].State[j].color.z = (object3ds[i].State[j].endColor.z - object3ds[i].State[j].startColor.z) / f;
					object3ds[i].State[j].color.w = (object3ds[i].State[j].endColor.w - object3ds[i].State[j].startColor.w) / f;
					object3ds[i].State[j].color.x = object3ds[i].State[j].color.x + object3ds[i].State[j].startColor.x;
					object3ds[i].State[j].color.y = object3ds[i].State[j].color.y + object3ds[i].State[j].startColor.y;
					object3ds[i].State[j].color.z = object3ds[i].State[j].color.z + object3ds[i].State[j].startColor.z;
					object3ds[i].State[j].color.w = object3ds[i].State[j].color.w + object3ds[i].State[j].startColor.w;
					//colorを0～1に丸める
					object3ds[i].State[j].color = ColorConvert(object3ds[i].State[j].color);

					object3ds[i].State[j].now_frame++;
					//加速度を加算
					object3ds[i].State[j].velocity.x = object3ds[i].State[j].velocity.x + object3ds[i].State[j].accel.x;
					object3ds[i].State[j].velocity.y = object3ds[i].State[j].velocity.y + object3ds[i].State[j].accel.y;
					object3ds[i].State[j].velocity.z = object3ds[i].State[j].velocity.z + object3ds[i].State[j].accel.z;
					//移動
					object3ds[i].State[j].position.x = object3ds[i].State[j].position.x + object3ds[i].State[j].velocity.x;
					object3ds[i].State[j].position.y = object3ds[i].State[j].position.y + object3ds[i].State[j].velocity.y;
					object3ds[i].State[j].position.z = object3ds[i].State[j].position.z + object3ds[i].State[j].velocity.z;
				}
				else if (object3ds[i].State[j].Type == P_CIRCULAR) {
					f = (float)object3ds[i].State[j].end_frame / (float)object3ds[i].State[j].now_frame;
					object3ds[i].State[j].scale = (object3ds[i].State[j].end_scale - object3ds[i].State[j].start_scale) / f;
					object3ds[i].State[j].scale += object3ds[i].State[j].start_scale;

					object3ds[i].State[j].color.x = (object3ds[i].State[j].endColor.x - object3ds[i].State[j].startColor.x) / f;
					object3ds[i].State[j].color.y = (object3ds[i].State[j].endColor.y - object3ds[i].State[j].startColor.y) / f;
					object3ds[i].State[j].color.z = (object3ds[i].State[j].endColor.z - object3ds[i].State[j].startColor.z) / f;
					object3ds[i].State[j].color.w = (object3ds[i].State[j].endColor.w - object3ds[i].State[j].startColor.w) / f;
					object3ds[i].State[j].color.x = object3ds[i].State[j].color.x + object3ds[i].State[j].startColor.x;
					object3ds[i].State[j].color.y = object3ds[i].State[j].color.y + object3ds[i].State[j].startColor.y;
					object3ds[i].State[j].color.z = object3ds[i].State[j].color.z + object3ds[i].State[j].startColor.z;
					object3ds[i].State[j].color.w = object3ds[i].State[j].color.w + object3ds[i].State[j].startColor.w;
					//colorを0～1に丸める
					object3ds[i].State[j].color = ColorConvert(object3ds[i].State[j].color);

					object3ds[i].State[j].now_frame++;
					//回転
					float XR = object3ds[i].State[j].Radius * cos((float)object3ds[i].State[j].now_frame / (float)object3ds[i].State[j].RotaFlame);
					float ZR = object3ds[i].State[j].Radius * sin((float)object3ds[i].State[j].now_frame / (float)object3ds[i].State[j].RotaFlame);

					//加速度を加算
					object3ds[i].State[j].velocity.x = object3ds[i].State[j].velocity.x + object3ds[i].State[j].accel.x;
					object3ds[i].State[j].velocity.y = object3ds[i].State[j].velocity.y + object3ds[i].State[j].accel.y;
					object3ds[i].State[j].velocity.z = object3ds[i].State[j].velocity.z + object3ds[i].State[j].accel.z;
					//移動
					//Centerの移動
					if(object3ds[i].State[j].emitter != nullptr){
						object3ds[i].State[j].Center.x = *&object3ds[i].State[j].emitter->pos.x;
						object3ds[i].State[j].Center.z = *&object3ds[i].State[j].emitter->pos.z;
					}
					else {
						object3ds[i].State[j].Center.x = object3ds[i].State[j].Center.x + object3ds[i].State[j].velocity.x;
						object3ds[i].State[j].Center.z = object3ds[i].State[j].Center.z + object3ds[i].State[j].velocity.z;
					}
					//回転の加算
					object3ds[i].State[j].position.x = object3ds[i].State[j].Center.x + XR;
					object3ds[i].State[j].position.z = object3ds[i].State[j].Center.z + ZR;

					object3ds[i].State[j].position.y = object3ds[i].State[j].position.y + object3ds[i].State[j].velocity.y;
				}
			}
			else {
				//粒の寿命が尽きたとき
				object3ds[i].State.erase(object3ds[i].State.begin() + j);
			}
		}
		//属するパーティクルが無くなったとき
		if (object3ds[i].State.size() == 0) {
			object3ds.erase(object3ds.begin() + i);
		}
	}

	DrawCount = 0;

	for (int i = 0; i < object3ds.size(); i++) {
		ParticleVertexPos *vertMap = nullptr;
		result = object3ds[i].vertBuff->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(result)) {
			// パーティクルの情報を1つずつ反映
			for (int j = 0; j < object3ds[i].State.size(); j++) {
				// 座標
				vertMap[j].pos = object3ds[i].State[j].position;
				vertMap[j].scale = object3ds[i].State[j].scale;
				vertMap[j].color = object3ds[i].State[j].color;
				// 次の頂点へ
				//vertMap++;

				DrawCount++;
			}
			object3ds[i].vertBuff->Unmap(0, nullptr);
		}

		//定数バッファへデータ転送
		ParticleConstBufferData *constMap0 = nullptr;
		if (SUCCEEDED(object3ds[i].constBuffB0->Map(0, nullptr, (void **)&constMap0))) {
			constMap0->mat = matView * matProjection;
			constMap0->matBillboard = Camera::matBillboard;
			object3ds[i].constBuffB0->Unmap(0, nullptr);
		}
	}


}

void ParticleManager::ParDrawobject3d(ParticleObj *object) {
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &object->vbView);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, object->constBuffB0->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		object->texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画コマンド
	DirectXBase::cmdList->DrawInstanced((UINT)object->State.size(), 1, 0, 0);
}

void ParticleManager::earthquakeCreate(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float R, UINT texNum, UINT ShaderNum) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.velocity = vel;
	par.accel = acc;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;

	const float width = 10.0f;

	float Range = (float)(rand() % (int)R);
	float Sphere = (float)(rand() % 360);

	for (int i = 0; i < count; i++) {
		//pos.x += MovePower * sin(player->rotation.y * M_PI / 180.0f);
		//pos.z += MovePower * cos(player->rotation.y * M_PI / 180.0f);
		Range = (float)(rand() % (int)R);
		Sphere = (float)(rand() % 360);
		par.position = XMFLOAT3{ Range * sinf(Sphere * M_PI_F / 180.0f),
			pos.y,
			Range * cosf(Sphere * M_PI_F / 180.0f) };
		obj.State.emplace_back(par);
	}

	object3ds.push_back(obj);

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * object3ds[object3ds.size() - 1].State.size() * object3ds[object3ds.size() - 1].MaxParticle),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].vertBuff)
	);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[(int)object3ds.size() - 1].State.size() * (UINT)object3ds[(int)object3ds.size() - 1].MaxParticle;
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);
	//int count = object3ds.size() - 1;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}



void ParticleManager::earthquakeCreate2(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, UINT texNum, UINT ShaderNum) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.velocity = vel;
	par.accel = acc;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;

	float widthX = RX * 2;
	float widthY = 1.0f;
	float widthZ = RZ * 2;
	for (int i = 0; i < count; i++) {
		par.position = XMFLOAT3{ (float)rand() / RAND_MAX * widthX - widthX / 2.0f + pos.x,
			(float)rand() / RAND_MAX * widthY - widthY / 2.0f + pos.y,
			(float)rand() / RAND_MAX * widthZ - widthZ / 2.0f + pos.z };
		obj.State.emplace_back(par);
	}

	object3ds.push_back(obj);

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * object3ds[object3ds.size() - 1].State.size() * object3ds[object3ds.size() - 1].MaxParticle),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].vertBuff)
	);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[(int)object3ds.size() - 1].State.size() * (UINT)object3ds[(int)object3ds.size() - 1].MaxParticle;
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);
	//int count = object3ds.size() - 1;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}


void ParticleManager::earthquakeCreate3(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, float Rot, UINT texNum, UINT ShaderNum) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.velocity = vel;
	par.accel = acc;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;

	float widthX = RX * 2;
	float widthY = 1.0f;
	float widthZ = RZ * 2;

	//x'=xcosθ-ysinθ
	//y'=xsinθ+ycosθ
	Rot = -Rot;
	for (int i = 0; i < count; i++) {
		par.position = XMFLOAT3{ (float)rand() / RAND_MAX * widthX - widthX / 2.0f + pos.x,
			(float)rand() / RAND_MAX * widthY - widthY / 2.0f + pos.y,
			(float)rand() / RAND_MAX * widthZ - widthZ / 2.0f + pos.z };

		par.position.x = ((par.position.x - pos.x) * cosf(Rot * M_PI_F / 180.0f)) + ((par.position.z - pos.z) * sinf(Rot * M_PI_F / 180.0f)) + pos.x;
		par.position.z = ((par.position.x - pos.x) * sinf(Rot * M_PI_F / 180.0f)) + ((par.position.z - pos.z) * cosf(Rot * M_PI_F / 180.0f)) + pos.z;

		/*par.position.x = par.position.x * sin(Rot * M_PI / 180.0f);
		par.position.z = par.position.z * cos(Rot * M_PI / 180.0f);*/

		obj.State.emplace_back(par);
	}

	object3ds.push_back(obj);

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * object3ds[object3ds.size() - 1].State.size() * object3ds[object3ds.size() - 1].MaxParticle),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].vertBuff)
	);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[(int)object3ds.size() - 1].State.size() * (UINT)object3ds[(int)object3ds.size() - 1].MaxParticle;
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);
	//int count = object3ds.size() - 1;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}



void ParticleManager::CreateRandVel(std::string tag, XMFLOAT3 pos, XMFLOAT3 velMIN, XMFLOAT3 velMAX, XMFLOAT3 accMIN, XMFLOAT3 accMAX, int lifetime, float start_scale, float end_scale, int count, UINT texNum, UINT ShaderNum, XMFLOAT4 startColor,
	XMFLOAT4 endColor) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.position = pos;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;
	par.startColor = startColor;
	par.endColor = endColor;

	XMFLOAT3 velRange = XMFLOAT3(velMAX.x - velMIN.x, velMAX.y - velMIN.y, velMAX.z - velMIN.z);
	XMFLOAT3 accRange = XMFLOAT3(accMAX.x - accMIN.x, accMAX.y - accMIN.y, accMAX.z - accMIN.z);
	for (int i = 0; i < count; i++) {
		par.velocity = XMFLOAT3{ (float)rand() / RAND_MAX * velRange.x - velRange.x / 2.0f + velMIN.x,
			(float)rand() / RAND_MAX * velRange.y - velRange.y / 2.0f + velMIN.y,
			(float)rand() / RAND_MAX * velRange.z - velRange.z / 2.0f + velMIN.z};
		par.accel = XMFLOAT3{ (float)rand() / RAND_MAX * accRange.x - accRange.x / 2.0f + accMIN.x,
			(float)rand() / RAND_MAX * accRange.y - accRange.y / 2.0f + accMIN.y,
			(float)rand() / RAND_MAX * accRange.z - accRange.z / 2.0f + accMIN.z};

		obj.State.emplace_back(par);
	}

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * obj.State.size()),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj.vertBuff)
	);

	object3ds.push_back(obj);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[(int)object3ds.size() - 1].State.size();
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}

void ParticleManager::CreateCIRCULARParticle(std::string tag, int count, XMFLOAT3 CenterPos, XMFLOAT3 vel, XMFLOAT3 acc, float Radius, int RotaFlame, int lifetime, float start_scale, float end_scale, UINT texNum, pEmitter *emitter, UINT ShaderNum, XMFLOAT4 startColor, XMFLOAT4 endColor) {
	ParticleObj obj;
	obj.tag = tag;
	obj.texNumber = texNum;
	obj.shaderNumber = ShaderNum;
	SingleParticle par;
	par.Center = CenterPos;
	par.end_frame = lifetime;
	par.start_scale = start_scale;
	par.end_scale = end_scale;
	par.Radius = Radius;
	par.RotaFlame = RotaFlame;
	par.velocity = vel;
	par.accel = acc;
	par.startColor = startColor;
	par.endColor = endColor;
	par.Type = P_CIRCULAR;
	par.emitter = emitter;
	
	for (int i = 0; i < count; i++) {
		obj.State.emplace_back(par);
	}

	object3ds.push_back(obj);

	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleVertexPos) * object3ds[(int)object3ds.size() - 1].State.size() * object3ds[(int)object3ds.size() - 1].MaxParticle),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].vertBuff)
	);

	object3ds[object3ds.size() - 1].vbView.BufferLocation = object3ds[object3ds.size() - 1].vertBuff->GetGPUVirtualAddress();
	object3ds[object3ds.size() - 1].vbView.SizeInBytes = sizeof(ParticleVertexPos) * (UINT)object3ds[object3ds.size() - 1].State.size() * (UINT)object3ds[object3ds.size() - 1].MaxParticle;
	object3ds[object3ds.size() - 1].vbView.StrideInBytes = sizeof(ParticleVertexPos);
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ParticleConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&object3ds[object3ds.size() - 1].constBuffB0)
	);
}
