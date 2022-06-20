#pragma once
#include <Windows.h>
#include "ExtendedRandom.h"

/// <summary>
/// �}�E�X�J�[�\���̕\��/��\����؂�ւ���
/// </summary>
/// <param name="Flag">true or false</param>
static void CursorShow(bool Flag) {
	if (Flag) {
		int s = ShowCursor(TRUE);
		while (s < 1) {
			s = ShowCursor(TRUE);
		}
	}
	else {
		int s = ShowCursor(FALSE);
		while (s > -1) {
			s = ShowCursor(FALSE);
		}
	}
}

/// <summary>
/// XMVECTOR��XMFLOAT3�ɕϊ�����
/// </summary>
/// <param name="vec">�l</param>
/// <returns></returns>
static inline DirectX::XMFLOAT3 ConvertXMFLOAT3(const DirectX::XMVECTOR &vec) {
	DirectX::XMFLOAT3 result;
	XMStoreFloat3(&result, vec);
	return result;
}

/// <summary>
/// ���W����ʓ��ɂ��邩
/// </summary>
/// <param name="WorldPos">���W</param>
/// <param name="ViewMat">�r���[�s��</param>
/// <param name="ProjMat">�v���W�F�N�V�����s��</param>
/// <param name="WinSize">�E�B���h�E�T�C�Y</param>
/// <returns></returns>
static inline bool InScreen(const DirectX::XMFLOAT3 WorldPos,
	const DirectX::XMMATRIX &ViewMat, const DirectX::XMMATRIX &ProjMat, const DirectX::XMFLOAT2 &WinSize) {

	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	float w = (float)WinSize.x / 2.0f;
	float h = (float)WinSize.y / 2.0f;

	DirectX::XMMATRIX viewport = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	DirectX::XMVECTOR screenPos;
	DirectX::XMVECTOR WorldPosV = XMLoadFloat3(&WorldPos);
	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	WorldPosV = VTransform(WorldPosV, ViewMat);
	WorldPosV = VTransform(WorldPosV, ProjMat);

	// z�Ŋ�����-1~1�͈̔͂Ɏ��߂�
	WorldPosV.m128_f32[0] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[1] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[2] /= WorldPosV.m128_f32[2];
	// �X�N���[���ϊ�
	screenPos = VTransform(WorldPosV, viewport);

	DirectX::XMFLOAT2 center;
	XMStoreFloat2(&center, screenPos);

	if (center.x < 0)return false;
	if (WinSize.x < center.x)return false;
	if (center.y < 0)return false;
	if (WinSize.y < center.y)return false;

	return true;
}