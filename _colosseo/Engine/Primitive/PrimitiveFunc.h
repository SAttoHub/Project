#pragma once
#include "Primitive2D.h"
#include "Primitive3D.h"

/// <summary>
/// �X�N���[����ɔ|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
static void DrawBox(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color);
}

/// <summary>
/// �X�N���[����ɔ|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
static void DrawBox(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color, false);
}

/// <summary>
/// 3D��ԏ�ɔ|����`�悷��
/// �܂�Z���W�Ή����ĂȂ��̂Ŏ΂߃|���S���͖����ł��B
/// �������Ŏw�肵��Z���W�̒l���K������܂��B
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
static void DrawBox3D(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color, true);
}

/// <summary>
/// �X�N���[����ɐ���`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
static void DrawLine(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color) {
	//Primitive2D::Instance()->DrawLine(pos1, pos2, color);
}

#pragma region DrawGraph

/// <summary>
/// �X�N���[����ɉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
static void DrawGraph(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
}

/// <summary>
/// �X�N���[����ɉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
static void DrawGraph(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, false);
}

/// <summary>
/// 3D��ԏ�ɉ摜�t���̔|����`�悷��
/// �܂�Z���W�Ή����ĂȂ��̂Ŏ΂߃|���S���͖����ł��B
/// �������Ŏw�肵��Z���W�̒l���K������܂��B
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
static void DrawGraph3D(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, true);
}

/// <summary>
/// �X�N���[����ɃA���t�@�l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="alpha">�A���t�@�l</param>
static void DrawGraphEffAlpha(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha });
}

/// <summary>
/// �X�N���[����ɃA���t�@�l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="alpha">�A���t�@�l</param>
static void DrawGraphEffAlpha(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, false);
}

/// <summary>
/// 3D��ԏ�ɃA���t�@�l���w�肵�ĉ摜�t���̔|����`�悷��
/// �܂�Z���W�Ή����ĂȂ��̂Ŏ΂߃|���S���͖����ł��B
/// �������Ŏw�肵��Z���W�̒l���K������܂��B
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="alpha">�A���t�@�l</param>
static void DrawGraph3DEffAlpha(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, true);
}

/// <summary>
/// �X�N���[����ɐF�e���l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="color">�e��������F</param>
static void DrawGraphEffColor(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color);
}

/// <summary>
/// �X�N���[����ɐF�e���l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="color">�e��������F</param>
static void DrawGraphEffColor(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color, false);
}

/// <summary>
/// 3D��ԏ�ɐF�e���l���w�肵�ĉ摜�t���̔|����`�悷��
/// �܂�Z���W�Ή����ĂȂ��̂Ŏ΂߃|���S���͖����ł��B
/// �������Ŏw�肵��Z���W�̒l���K������܂��B
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="color">�e��������F</param>
static void DrawGraph3DEffColor(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color, true);
}

#pragma region DrawGraphBack

/// <summary>
/// �X�N���[����ɉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
static void DrawGraphBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
}

/// <summary>
/// �X�N���[����ɉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
static void DrawGraphBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, false);
}

/// <summary>
/// �X�N���[����ɃA���t�@�l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="alpha">�A���t�@�l</param>
static void DrawGraphEffAlphaBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha });
}

/// <summary>
/// �X�N���[����ɃA���t�@�l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="alpha">�A���t�@�l</param>
static void DrawGraphEffAlphaBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, false);
}

/// <summary>
/// �X�N���[����ɐF�e���l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="color">�e��������F</param>
static void DrawGraphEffColorBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, color);
}

/// <summary>
/// �X�N���[����ɐF�e���l���w�肵�ĉ摜�t���̔|����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="TextureNumber">�摜�n���h��</param>
/// <param name="color">�e��������F</param>
static void DrawGraphEffColorBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, color, false);
}
#pragma endregion
#pragma endregion

#pragma region 3D
/// <summary>
/// 3D��ԏ�ɗ����̂�`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, XMFLOAT3(1, 1, 1), XMFLOAT3(0,0,0), color, Lighting);
}

/// <summary>
/// 3D��ԏ�ɗ����̂�`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawCubeRotate(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, XMFLOAT3(1, 1, 1), rotate, color, Lighting);
}


/// <summary>
/// 3D��ԏ�ɗ����̂�`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawCubeScaleRotate(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, scale, rotate, color, Lighting);
}

/// <summary>
/// 3D��ԏ�ɐ�����`�悷��
/// </summary>
/// <param name="pos1">�n�_�ƂȂ���W�P</param>
/// <param name="pos2">�I�_�ƂȂ���W�Q</param>
/// <param name="color">�F</param>
static void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive3D::Instance()->DrawLine(pos1, pos2, color);
}

/// <summary>
/// 3D��ԏ�ɃO���b�h����`�悷��
/// </summary>
/// <param name="pos1">��ƂȂ钸�_���W�P</param>
/// <param name="pos2">��ƂȂ钸�_���W�Q</param>
/// <param name="interval">�Ԋu</param>
/// <param name="color">�F</param>
static void DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval, XMFLOAT4 color) {
	Primitive3D::Instance()->DrawGrid(pos1, pos2, interval, color);
}

/// <summary>
/// 3D��ԏ�Ɏl�ʑ̂�`�悷��
/// </summary>
/// <param name="pos1">���_���W�P</param>
/// <param name="pos2">���_���W�Q</param>
/// <param name="pos3">���_���W�R</param>
/// <param name="pos4">���_���W�S</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawTetrahedron(pos1, pos2, pos3, pos4, color, Lighting);
}

/// <summary>
/// 3D��ԏ�ɋ��Ŕ͈͂��w�肵�Đ����ʑ̂�`�悷��
/// </summary>
/// <param name="pos">���S���W</param>
/// <param name="Radius">���a</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawOctahedron(pos, Radius, color, Lighting);
}

/// <summary>
/// 3D��ԏ�ɋ��Ŕ͈͂��w�肵�Đ���\�ʑ̂�`�悷��
/// </summary>
/// <param name="pos">���S���W</param>
/// <param name="Radius">���a</param>
/// <param name="color">�F</param>
/// <param name="Lighting">���C�e�B���O��L���ɂ��邩</param>
static void DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawIcosahedron(pos, Radius, color, Lighting);
}
#pragma endregion