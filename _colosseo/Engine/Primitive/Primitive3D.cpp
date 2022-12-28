#include "Primitive3D.h"
#include "..\..\Camera.h"
#include "..\..\TexManager.h"

void Primitive3D::Initialize()
{
	//頂点バッファの生成
	HRESULT result;

	// 各プリミティブの実体生成
	if (CubeUse)	CubePrim = std::make_unique<CubePrimData>();
	if (LineUse)	LinePrim = std::make_unique<LinePrimData>();
	if (TetraUse)	TetraPrim = std::make_unique<TetrahedronPrimData>();
	if (OctaUse)	OctaPrim = std::make_unique<OctahedronPrimData>();
	if (IcosaUse)	IcosaPrim = std::make_unique<IcosahedronPrimData>();

#pragma region 箱プリミティブ
	if (CubeUse) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(CubePrimitive3D::Cube) * CubePrim->MaxPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&CubePrim->VertBuff)
		);
		CubePrim->vbView.BufferLocation = CubePrim->VertBuff->GetGPUVirtualAddress();
		CubePrim->vbView.SizeInBytes = sizeof(CubePrimitive3D::Cube) * CubePrim->MaxPrimitives;
		CubePrim->vbView.StrideInBytes = sizeof(CubePrimitive3D::Cube);
	}
#pragma endregion
#pragma region 線プリミティブ
	if (LineUse) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(LinePrimitive3D::Line) * LinePrim->MaxPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&LinePrim->VertBuff)
		);
		LinePrim->vbView.BufferLocation = LinePrim->VertBuff->GetGPUVirtualAddress();
		LinePrim->vbView.SizeInBytes = sizeof(LinePrimitive3D::Line) * LinePrim->MaxPrimitives;
		LinePrim->vbView.StrideInBytes = sizeof(LinePrimitive3D::Line);
	}
#pragma endregion
#pragma region 正四面体プリミティブ
	if (TetraUse) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(TetrahedronPrimitive::Tetrahedron) * TetraPrim->MaxPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&TetraPrim->VertBuff)
		);
		TetraPrim->vbView.BufferLocation = TetraPrim->VertBuff->GetGPUVirtualAddress();
		TetraPrim->vbView.SizeInBytes = sizeof(TetrahedronPrimitive::Tetrahedron) * TetraPrim->MaxPrimitives;
		TetraPrim->vbView.StrideInBytes = sizeof(TetrahedronPrimitive::Tetrahedron);
	}
#pragma endregion
#pragma region 正八面体プリミティブ
	if (OctaUse) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(OctahedronPrimitive::Octahedron) * OctaPrim->MaxPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&OctaPrim->VertBuff)
		);
		OctaPrim->vbView.BufferLocation = OctaPrim->VertBuff->GetGPUVirtualAddress();
		OctaPrim->vbView.SizeInBytes = sizeof(OctahedronPrimitive::Octahedron) * OctaPrim->MaxPrimitives;
		OctaPrim->vbView.StrideInBytes = sizeof(OctahedronPrimitive::Octahedron);
	}
#pragma endregion
#pragma region 正二十面体プリミティブ
	if (IcosaUse) {
		result = DirectXBase::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(IcosahedronPrimitive3D::Icosahedron) * IcosaPrim->MaxPrimitives),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&IcosaPrim->VertBuff)
		);
		IcosaPrim->vbView.BufferLocation = IcosaPrim->VertBuff->GetGPUVirtualAddress();
		IcosaPrim->vbView.SizeInBytes = sizeof(IcosahedronPrimitive3D::Icosahedron) * IcosaPrim->MaxPrimitives;
		IcosaPrim->vbView.StrideInBytes = sizeof(IcosahedronPrimitive3D::Icosahedron);
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
	

	if (FAILED(result)) {
		assert(0);
	}

	if (CubeUse)	CubePrim->SetupCubePrimitive();
	if (LineUse)	LinePrim->SetupLinePrimitive();
	if (TetraUse)	TetraPrim->SetupTetrahedronPrimitive();
	if (OctaUse)	OctaPrim->SetupOctahedronPrimitive();
	if (IcosaUse)	IcosaPrim->SetupIcosahedronPrimitive();

	LineTexture = TexManager::LoadTexture("Resource/image/line.png");
}

void Primitive3D::Update()
{
	if (CubeUse) {
		for (auto& data : CubePrim->Data) {
			data.Data.pos1 = XMFLOAT3{ 0,0,0 };
			data.Data.pos2 = XMFLOAT3{ 0,0,0 };
			data.Data.color = XMFLOAT4{ 0,0,0,0 }; // プリミティブの色
			data.Data.Lighting = false; // ライティングが有効か
			data.Active = false;
		}
	}
	if (LineUse) {
		for (auto& data : LinePrim->Data) {
			data.Data.pos1 = XMFLOAT3{ 0,0,0 };
			data.Data.pos2 = XMFLOAT3{ 0,0,0 };
			data.Data.color = XMFLOAT4{ 0,0,0,0 }; // プリミティブの色
			data.Active = false;
		}
	}
	if (OctaUse) {
		for (auto& data : OctaPrim->Data) {
			data.Data.pos = XMFLOAT3{ 0,0,0 };
			data.Data.Radius = 0;
			data.Data.color = XMFLOAT4{ 0,0,0,0 }; // プリミティブの色
			data.Data.Lighting = false; // ライティングが有効か
			data.Active = false;
		}
	}
	if (IcosaUse) {
		for (auto& data : IcosaPrim->Data) {
			data.Data.pos = XMFLOAT3{ 0,0,0 };
			data.Data.Radius = 0;
			data.Data.color = XMFLOAT4{ 0,0,0,0 }; // プリミティブの色
			data.Data.Lighting = false; // ライティングが有効か
			data.Active = false;
		}
	}
	if (TetraUse) {
		for (auto& data : TetraPrim->Data) {
			data.Data.pos1 = XMFLOAT3{ 0,0,0 };
			data.Data.pos2 = XMFLOAT3{ 0,0,0 };
			data.Data.pos3 = XMFLOAT3{ 0,0,0 };
			data.Data.pos4 = XMFLOAT3{ 0,0,0 };
			data.Data.color = XMFLOAT4{ 0,0,0,0 }; // プリミティブの色
			data.Data.Lighting = false; // ライティングが有効か
			data.Active = false;
		}
	}
}

void Primitive3D::CubeDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (int i = 0; i < CubePrim->MaxPrimitives; i++) {
		if (CubePrim->Data[i].Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	CubePrimitive3D::Cube *vertMap = nullptr;
	result = CubePrim->VertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& data : CubePrim->Data) {
			if (data.Active) {
				vertMap->pos1 =		 data.Data.pos1;
				vertMap->pos2 =		 data.Data.pos2;
				vertMap->Scale =	 data.Data.Scale;
				vertMap->Rotate =	 data.Data.Rotate;
				vertMap->color =	 data.Data.color;
				vertMap->Lighting =	 data.Data.Lighting;
				vertMap++;
			}
		}
	}
	CubePrim->VertBuff->Unmap(0, nullptr);

	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//ルートシグネチャ
	DirectXBase::cmdList->SetGraphicsRootSignature(CubePrim->Rootsignature.Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(CubePrim->Pipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &CubePrim->vbView);
	//デスクリプタヒープをセット
	//ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	//DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	/*DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		Tex,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));*/

	// ライトの描画
	//DirectX3dObject::lightGroup->Draw(DirectXBase::cmdList.Get(), 3);
	//描画コマンド
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::LineDrawAll()
{
	HRESULT result;
	int activeCount = 0;
	for (auto& data : LinePrim->Data) {
		if (data.Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	LinePrimitive3D::Line *vertMap = nullptr;
	result = LinePrim->VertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& data : LinePrim->Data) {
			if (data.Active) {
				vertMap->pos1 =  data.Data.pos1;
				vertMap->pos2 =  data.Data.pos2;
				vertMap->color = data.Data.color;
				vertMap++;
			}
		}
	}
	LinePrim->VertBuff->Unmap(0, nullptr);
	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//ルートシグネチャ
	DirectXBase::cmdList->SetGraphicsRootSignature(LinePrim->Rootsignature.Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(LinePrim->Pipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &LinePrim->vbView);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//シェーダーリソースビュー
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		LineTexture,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//描画コマンド
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawTetrahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (auto& data : TetraPrim->Data) {
		if (data.Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	TetrahedronPrimitive::Tetrahedron *vertMap = nullptr;
	result = TetraPrim->VertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& data : TetraPrim->Data) {
			if (data.Active) {
				vertMap->pos1 =		data.Data.pos1;
				vertMap->pos2 =		data.Data.pos2;
				vertMap->pos3 =		data.Data.pos3;
				vertMap->pos4 =		data.Data.pos4;
				vertMap->color =	data.Data.color;
				vertMap->Lighting = data.Data.Lighting;
				vertMap++;
			}
		}
	}
	TetraPrim->VertBuff->Unmap(0, nullptr);
	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//ルートシグネチャ
	DirectXBase::cmdList->SetGraphicsRootSignature(TetraPrim->Rootsignature.Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(TetraPrim->Pipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &TetraPrim->vbView);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//描画コマンド
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawOctahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (auto& data : OctaPrim->Data) {
		if (data.Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	OctahedronPrimitive::Octahedron *vertMap = nullptr;
	result = OctaPrim->VertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& data : OctaPrim->Data) {
			if (data.Active) {
				vertMap->pos =		data.Data.pos;
				vertMap->Radius =	data.Data.Radius;
				vertMap->color =	data.Data.color;
				vertMap->Lighting = data.Data.Lighting;
				vertMap++;
			}
		}
	}
	OctaPrim->VertBuff->Unmap(0, nullptr);
	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//ルートシグネチャ
	DirectXBase::cmdList->SetGraphicsRootSignature(OctaPrim->Rootsignature.Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(OctaPrim->Pipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &OctaPrim->vbView);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//描画コマンド
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::DrawIcosahedronAll()
{
	HRESULT result;
	int activeCount = 0;
	for (auto& data : IcosaPrim->Data) {
		if (data.Active) {
			activeCount++;
		}
	}
	if (activeCount == 0) {
		return;
	}
	IcosahedronPrimitive3D::Icosahedron *vertMap = nullptr;
	result = IcosaPrim->VertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& data : IcosaPrim->Data) {
			if (data.Active) {
				vertMap->pos =		data.Data.pos;
				vertMap->Radius =	data.Data.Radius;
				vertMap->color =	data.Data.color;
				vertMap->Lighting = data.Data.Lighting;
				vertMap++;
			}
		}
	}
	IcosaPrim->VertBuff->Unmap(0, nullptr);
	//定数バッファへデータ転送
	ConstBufferData *constMap0 = nullptr;
	if (SUCCEEDED(ConstBuff0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = Camera::matView * Camera::matProjection;
		constMap0->matBillboard = Camera::matBillboard;
		ConstBuff0->Unmap(0, nullptr);
	}
	//ビューポートの設定コマンド
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//シザー矩形の設定コマンド
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, LONG(DirectXBase::Win_Width), LONG(DirectXBase::Win_Height)));
	//ルートシグネチャ
	DirectXBase::cmdList->SetGraphicsRootSignature(IcosaPrim->Rootsignature.Get());
	//パイプラインステートの設定コマンド
	DirectXBase::cmdList->SetPipelineState(IcosaPrim->Pipelinestate.Get());
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//頂点バッファの設定
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &IcosaPrim->vbView);
	//定数バッファビューをセット
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, ConstBuff0->GetGPUVirtualAddress());
	//描画コマンド
	DirectXBase::cmdList->DrawInstanced(activeCount, 1, 0, 0);
}

void Primitive3D::Draw()
{
	if (TetraUse)DrawTetrahedronAll();
	if (OctaUse)DrawOctahedronAll();
	if (IcosaUse)DrawIcosahedronAll();
	if (CubeUse)CubeDrawAll();
	if (LineUse)LineDrawAll();
}

void Primitive3D::DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting) {
	if (!CubeUse) {
		return;
	}
	int Num = 0;
	for (auto& data : CubePrim->Data) {
		if (data.Active == false) break;
		Num++;
	}
	assert(Num <= CubePrim->MaxPrimitives);

	this->CubePrim->Data[Num].Data.pos1 = pos1;
	this->CubePrim->Data[Num].Data.pos2 = pos2;
	this->CubePrim->Data[Num].Data.Scale = scale;
	this->CubePrim->Data[Num].Data.Rotate = rotate;
	this->CubePrim->Data[Num].Data.color = color;
	this->CubePrim->Data[Num].Data.Lighting = Lighting;
	CubePrim->Data[Num].Active = true;
}

void Primitive3D::DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	if (!LineUse) {
		return;
	}
	int Num = 0;
	for (auto& data : LinePrim->Data) {
		if (data.Active == false) break;
		Num++;
	}
	assert(Num <= LinePrim->MaxPrimitives);

	this->LinePrim->Data[Num].Data.pos1 = pos1;
	this->LinePrim->Data[Num].Data.pos2 = pos2;
	this->LinePrim->Data[Num].Data.color = color;
	LinePrim->Data[Num].Active = true;
}

void Primitive3D::DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting)
{
	if (!TetraUse) {
		return;
	}
	int Num = 0;
	for (auto& data : TetraPrim->Data) {
		if (data.Active == false) break;
		Num++;
	}
	assert(Num <= TetraPrim->MaxPrimitives);

	this->TetraPrim->Data[Num].Data.pos1 = pos1;
	this->TetraPrim->Data[Num].Data.pos2 = pos2;
	this->TetraPrim->Data[Num].Data.pos3 = pos3;
	this->TetraPrim->Data[Num].Data.pos4 = pos4;
	this->TetraPrim->Data[Num].Data.color = color;
	this->TetraPrim->Data[Num].Data.Lighting = Lighting;
	TetraPrim->Data[Num].Active = true;
}

void Primitive3D::DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting)
{
	if (!OctaUse) {
		return;
	}
	int Num = 0;
	for (auto& data : OctaPrim->Data) {
		if (data.Active == false) break;
		Num++;
	}
	assert(Num <= OctaPrim->MaxPrimitives);

	this->OctaPrim->Data[Num].Data.pos = pos;
	this->OctaPrim->Data[Num].Data.Radius = Radius;
	this->OctaPrim->Data[Num].Data.color = color;
	this->OctaPrim->Data[Num].Data.Lighting = Lighting;
	OctaPrim->Data[Num].Active = true;
}

void Primitive3D::DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting)
{
	if (!IcosaUse) {
		return;
	}
	int Num = 0;
	for (auto& data : IcosaPrim->Data) {
		if (data.Active == false) break;
		Num++;
	}
	assert(Num <= IcosaPrim->MaxPrimitives);

	this->IcosaPrim->Data[Num].Data.pos = pos;
	this->IcosaPrim->Data[Num].Data.Radius = Radius;
	this->IcosaPrim->Data[Num].Data.color = color;
	this->IcosaPrim->Data[Num].Data.Lighting = Lighting;
	IcosaPrim->Data[Num].Active = true;
}

void Primitive3D::DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval,XMFLOAT4 color)
{
	float min = 1;
	float max = 1;
	float count = 0;
	int kei = 1;
	//X軸
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
	//Z軸
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

void TetrahedronPrimData::SetupTetrahedronPrimitive()
{
	PrimitiveLoadShader::LoadShader(&vsBlob, L"Resource/shader/TetrahedronVertex.hlsl", "main", "vs_5_0");
	PrimitiveLoadShader::LoadShader(&gsBlob, L"Resource/shader/TetrahedronGeometry.hlsl", "main", "gs_5_0");
	PrimitiveLoadShader::LoadShader(&psBlob, L"Resource/shader/TetrahedronPixel.hlsl", "main", "ps_5_0");

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
	ID3DBlob* errorBlob = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = TetrahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(TetrahedronPrimitiveInputLayout);//_countof(nputLayout)
	//図形の形状を点に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&Rootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = Rootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&Pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void CubePrimData::SetupCubePrimitive()
{
	PrimitiveLoadShader::LoadShader(&vsBlob, L"Resource/shader/CubePrimitiveVertex.hlsl", "main", "vs_5_0");
	PrimitiveLoadShader::LoadShader(&gsBlob, L"Resource/shader/CubePrimitiveGeometry.hlsl", "main", "gs_5_0");
	PrimitiveLoadShader::LoadShader(&psBlob, L"Resource/shader/CubePrimitivePixel.hlsl", "main", "ps_5_0");

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
	ID3DBlob* errorBlob = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = CubePrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(CubePrimitiveInputLayout);//_countof(nputLayout)
	//図形の形状を点に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デスクリプタテーブルの設定
	//CD3DX12_DESCRIPTOR_RANGE descRangeCBV; //定数用
	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
	//descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	//descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&Rootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = Rootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&Pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void LinePrimData::SetupLinePrimitive()
{
	PrimitiveLoadShader::LoadShader(&vsBlob, L"Resource/shader/LinePrimitiveVertex.hlsl", "main", "vs_5_0");
	PrimitiveLoadShader::LoadShader(&gsBlob, L"Resource/shader/LinePrimitiveGeometry.hlsl", "main", "gs_5_0");
	PrimitiveLoadShader::LoadShader(&psBlob, L"Resource/shader/LinePrimitivePixel.hlsl", "main", "ps_5_0");

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
	ID3DBlob* errorBlob = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = LinePrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(LinePrimitiveInputLayout);//_countof(nputLayout)
	//図形の形状を点に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV; //テクスチャ用
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&Rootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = Rootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&Pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void OctahedronPrimData::SetupOctahedronPrimitive()
{
	PrimitiveLoadShader::LoadShader(&vsBlob, L"Resource/shader/OctahedronVertex.hlsl", "main", "vs_5_0");
	PrimitiveLoadShader::LoadShader(&gsBlob, L"Resource/shader/OctahedronGeometry.hlsl", "main", "gs_5_0");
	PrimitiveLoadShader::LoadShader(&psBlob, L"Resource/shader/OctahedronPixel.hlsl", "main", "ps_5_0");

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
	ID3DBlob* errorBlob = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = OctahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(OctahedronPrimitiveInputLayout);//_countof(nputLayout)
	//図形の形状を点に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&Rootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = Rootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&Pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void IcosahedronPrimData::SetupIcosahedronPrimitive()
{
	PrimitiveLoadShader::LoadShader(&vsBlob, L"Resource/shader/IcosahedronVertex.hlsl", "main", "vs_5_0");
	PrimitiveLoadShader::LoadShader(&gsBlob, L"Resource/shader/IcosahedronGeometry.hlsl", "main", "gs_5_0");
	PrimitiveLoadShader::LoadShader(&psBlob, L"Resource/shader/IcosahedronPixel.hlsl", "main", "ps_5_0");

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
	ID3DBlob* errorBlob = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //グラフィックスパイプラインの各ステージを設定する構造体
	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);
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
	gpipeline.InputLayout.pInputElementDescs = IcosahedronPrimitiveInputLayout;
	gpipeline.InputLayout.NumElements = _countof(IcosahedronPrimitiveInputLayout);//_countof(nputLayout)
	//図形の形状を点に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&Rootsignature));
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = Rootsignature.Get();
	//パイプラインステートの生成
	result = DirectXBase::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&Pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}
