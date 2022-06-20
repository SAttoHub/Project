#include "Sprite.h"

unsigned short Sprite::indices[6] = { 0,1,2,1,2,3, };
D3D12_INDEX_BUFFER_VIEW Sprite::ibView{};

void Sprite::DxSpriteBaceReset(Window *Win) {


	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);

	ID3D12Resource *indexBuff = nullptr;
	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Sprite::SpriteInit(SpriteData *sprdata) {
	SpriteVertex vertices[4] = {
		//�O
		{{0.0f, 100.0f, 0.0f},{0.0f,1.0f}}, //����
		{{0.0f, 0.0f, 0.0f},{0.0f,0.0f}}, //����
		{{100.0f, 100.0f, 0.0f},{1.0f,1.0f}},  //�E��
		{{100.0f, 0.0f, 0.0f},{1.0f,0.0f}},  //�E��
	};
	sprdata->UV[0] = { 0.0f,1.0f };
	sprdata->UV[1] = { 0.0f,0.0f };
	sprdata->UV[2] = { 1.0f,1.0f };
	sprdata->UV[3] = { 1.0f,0.0f };

	float width = 0;
	float height = 0;
	if (sprdata->AdjustSizeTexture == true) {
		D3D12_RESOURCE_DESC resDesc =
			TexManager::TextureData[sprdata->texNumber].TextureBuff->GetDesc();
		width = (float)resDesc.Width; //�摜�̉���
		height = (float)resDesc.Height; //�摜�̏c��
		sprdata->Size.x = width;
		sprdata->Size.y = height;
		vertices[0].pos = { 0.0f, height, 0.0f };
		vertices[1].pos = { 0.0f, 0.0f, 0.0f };
		vertices[2].pos = { width, height, 0.0f };
		vertices[3].pos = { width, 0.0f, 0.0f };
	}

	//���_�o�b�t�@
	HRESULT result;
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprdata->vertBuff)
	);
	SpriteVertex *vertMap = nullptr;
	result = sprdata->vertBuff->Map(0, nullptr, (void **)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //���W���R�s�[
	}
	//�}�b�v������
	sprdata->vertBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�r���[
	sprdata->vbView.BufferLocation = sprdata->vertBuff->GetGPUVirtualAddress();
	sprdata->vbView.SizeInBytes = sizeof(vertices);
	sprdata->vbView.StrideInBytes = sizeof(vertices[0]);
	//�萔�o�b�t�@
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstBufferData) + 0xff) & ~0xff), //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprdata->constBuffB0)
	);
	//��]
	sprdata->spriteMatProjection = XMMatrixOrthographicOffCenterLH(0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height, 0.0f, 0.0f, 1.0f);
	//���[���h�s��̍X�V
	sprdata->spriteMatWorld = XMMatrixIdentity();
	sprdata->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprdata->spriteRotation));
	sprdata->spriteMatWorld *= XMMatrixTranslationFromVector(sprdata->spritePosition);

	SpriteConstBufferData *constMap = nullptr;
	result = sprdata->constBuffB0->Map(0, nullptr, (void **)&constMap); //�}�b�s���O
	constMap->color = sprdata->constBufferData.color;
	constMap->mat = sprdata->spriteMatWorld * sprdata->spriteMatProjection; //�s��̍���
	sprdata->constBuffB0->Unmap(0, nullptr);
}

void Sprite::SpriteDraw(SpriteData *sprdata) {
	//�r���[�|�[�g�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height));
	//�V�U�[��`�̐ݒ�R�}���h
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)DirectXBase::Win_Width, (LONG)DirectXBase::Win_Height));
	switch (sprdata->ShaderNumber){
	case SpriteNormal:
		//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
		DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[SpriteNormal].Get());
		//���[�g�V�O�l�`���̐ݒ�R�}���h
		DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[SpriteNormal].Get());
		break;
	case SpriteNormal2:
		//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
		DirectXBase::cmdList->SetPipelineState(PipelineManager::pipelinestate[SpriteNormal2].Get());
		//���[�g�V�O�l�`���̐ݒ�R�}���h
		DirectXBase::cmdList->SetGraphicsRootSignature(PipelineManager::rootsignature[SpriteNormal2].Get());
		break;
	default:
		break;
	}
	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::TextureDescHeap.Get() };
	DirectXBase::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�v���~�e�B�u�`��̐ݒ�R�}���h(�O�p�`���X�g)
	DirectXBase::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//���_�o�b�t�@�̐ݒ�R�}���h
	DirectXBase::cmdList->IASetVertexBuffers(0, 1, &sprdata->vbView);

	DirectXBase::cmdList->IASetIndexBuffer(&ibView);


	//�萔�o�b�t�@�r���[���Z�b�g
	DirectXBase::cmdList->SetGraphicsRootConstantBufferView(0, sprdata->constBuffB0->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[
	DirectXBase::cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		TexManager::TextureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		sprdata->texNumber,
		DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//�`��R�}���h
	DirectXBase::cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::spriteChangeRect(SpriteData *sprdata, XMFLOAT2 texLeftTop, XMFLOAT2 texSize) {
	//�e�N�X�`�����擾
	D3D12_RESOURCE_DESC resDesc = TexManager::TextureData[sprdata->texNumber].TextureBuff->GetDesc();

	sprdata->texLeftTop = texLeftTop;
	sprdata->texSize = texSize;

	float tex_left = sprdata->texLeftTop.x / resDesc.Width;
	float tex_right = (sprdata->texLeftTop.x + sprdata->texSize.x) / resDesc.Width;
	float tex_top = sprdata->texLeftTop.y / resDesc.Height;
	float tex_bottom = (sprdata->texLeftTop.y + sprdata->texSize.y) / resDesc.Height;

	sprdata->UV[0] = { tex_left,	tex_bottom }; // ����
	sprdata->UV[1] = { tex_left,	tex_top }; // ����
	sprdata->UV[2] = { tex_right,	tex_bottom }; // �E��
	sprdata->UV[3] = { tex_right,	tex_top }; // �E��

	SpriteVertex vertices[4] = {
		//�O
		{{0.0f, 100.0f, 0.0f},sprdata->UV[0]}, //����
		{{0.0f, 0.0f, 0.0f},sprdata->UV[1]}, //����
		{{100.0f, 100.0f, 0.0f},sprdata->UV[2]},  //�E��
		{{100.0f, 0.0f, 0.0f},sprdata->UV[3]},  //�E��
	};

	vertices[0].pos = { 0.0f, sprdata->Size.y, 0.0f };
	vertices[1].pos = { 0.0f, 0.0f, 0.0f };
	vertices[2].pos = { sprdata->Size.x, sprdata->Size.y, 0.0f };
	vertices[3].pos = { sprdata->Size.x, 0.0f, 0.0f };


	SpriteVertex *vertMap = nullptr;
	HRESULT result;
	result = sprdata->vertBuff->Map(0, nullptr, (void **)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //���W���R�s�[
	}
	//�}�b�v������
	sprdata->vertBuff->Unmap(0, nullptr);
}

void Sprite::spriteChangeSize(SpriteData *sprdata, float width, float height) {
	SpriteVertex vertices[4] = {
		//�O
		{{0.0f, 100.0f, 0.0f},sprdata->UV[0]}, //����
		{{0.0f, 0.0f, 0.0f},sprdata->UV[1]}, //����
		{{100.0f, 100.0f, 0.0f},sprdata->UV[2]},  //�E��
		{{100.0f, 0.0f, 0.0f},sprdata->UV[3]},  //�E��
	};

	sprdata->Size.x = width;
	sprdata->Size.y = height;

	vertices[0].pos = { 0.0f, sprdata->Size.y, 0.0f };
	vertices[1].pos = { 0.0f, 0.0f, 0.0f };
	vertices[2].pos = { sprdata->Size.x, sprdata->Size.y, 0.0f };
	vertices[3].pos = { sprdata->Size.x, 0.0f, 0.0f };

	SpriteVertex *vertMap = nullptr;
	HRESULT result;
	result = sprdata->vertBuff->Map(0, nullptr, (void **)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //���W���R�s�[
	}
	//�}�b�v������
	sprdata->vertBuff->Unmap(0, nullptr);
}

void Sprite::spriteChangePos(SpriteData *sprdata, float X, float Y) {
	sprdata->spritePosition = { (float)X, (float)Y, 0, 0 };
	//��]
	sprdata->spriteMatProjection = XMMatrixOrthographicOffCenterLH(0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height, 0.0f, 0.0f, 1.0f);
	//���[���h�s��̍X�V
	sprdata->spriteMatWorld = XMMatrixIdentity();
	sprdata->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprdata->spriteRotation));
	sprdata->spriteMatWorld *= XMMatrixTranslationFromVector(sprdata->spritePosition);

	SpriteConstBufferData *constMap = nullptr;
	sprdata->constBuffB0->Map(0, nullptr, (void **)&constMap); //�}�b�s���O
	constMap->color = sprdata->constBufferData.color;
	constMap->mat = sprdata->spriteMatWorld * sprdata->spriteMatProjection; //�s��̍���
	sprdata->constBuffB0->Unmap(0, nullptr);
}

void Sprite::spriteChangePosRefCenter(SpriteData *sprdata, float X, float Y) {
	sprdata->spritePosition = { (float)X - sprdata->Size.x / 2.0f, (float)Y - sprdata->Size.y / 2.0f, 0, 0 };
	//��]
	sprdata->spriteMatProjection = XMMatrixOrthographicOffCenterLH(0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height, 0.0f, 0.0f, 1.0f);
	//���[���h�s��̍X�V
	sprdata->spriteMatWorld = XMMatrixIdentity();
	sprdata->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprdata->spriteRotation));
	sprdata->spriteMatWorld *= XMMatrixTranslationFromVector(sprdata->spritePosition);

	SpriteConstBufferData *constMap = nullptr;
	sprdata->constBuffB0->Map(0, nullptr, (void **)&constMap); //�}�b�s���O
	constMap->color = sprdata->constBufferData.color;
	constMap->mat = sprdata->spriteMatWorld * sprdata->spriteMatProjection; //�s��̍���
	sprdata->constBuffB0->Unmap(0, nullptr);
}

void Sprite::spriteChangeRotZ(SpriteData *sprdata, float RotZ) {
	sprdata->spriteRotation = RotZ;
	//��]
	sprdata->spriteMatProjection = XMMatrixOrthographicOffCenterLH(0.0f, DirectXBase::Win_Width, DirectXBase::Win_Height, 0.0f, 0.0f, 1.0f);
	//���[���h�s��̍X�V
	sprdata->spriteMatWorld = XMMatrixIdentity();
	sprdata->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprdata->spriteRotation));
	sprdata->spriteMatWorld *= XMMatrixTranslationFromVector(sprdata->spritePosition);

	SpriteConstBufferData *constMap = nullptr;
	sprdata->constBuffB0->Map(0, nullptr, (void **)&constMap); //�}�b�s���O
	constMap->color = sprdata->constBufferData.color;
	constMap->mat = sprdata->spriteMatWorld * sprdata->spriteMatProjection; //�s��̍���
	sprdata->constBuffB0->Unmap(0, nullptr);
}

SpriteData Sprite::SpriteCreate(int LT_X, int LT_Y, UINT texNum, UINT ShaderNum) {
	SpriteData spd;
	spd.spritePosition = { (float)LT_X, (float)LT_Y, 0, 0 };
	spd.AdjustSizeTexture = true;
	spd.texNumber = texNum;
	spd.ShaderNumber = ShaderNum;
	SpriteInit(&spd);
	return spd;
};

SpriteData Sprite::ExtendSpriteCreate(int LT_X, int LT_Y, int RB_X, int RB_Y, UINT texNum, UINT ShaderNum) {
	SpriteData spd;
	spd.spritePosition = { (float)LT_X, (float)LT_Y, 0, 0 };
	spd.texNumber = texNum;
	spd.ShaderNumber = ShaderNum;
	SpriteInit(&spd);
	spriteChangeSize(&spd, (float)(RB_X - LT_X), (float)(RB_Y - LT_Y));
	return spd;
};