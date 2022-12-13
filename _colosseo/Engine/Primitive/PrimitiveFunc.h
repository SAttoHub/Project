#pragma once
#include "Primitive2D.h"
#include "Primitive3D.h"

/// <summary>
/// スクリーン上に板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
static void DrawBox(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color);
}

/// <summary>
/// スクリーン上に板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
static void DrawBox(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color, false);
}

/// <summary>
/// 3D空間上に板ポリを描画する
/// まだZ座標対応してないので斜めポリゴンは無理です。
/// 第一引数で指定したZ座標の値が適応されます。
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
static void DrawBox3D(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawBox(pos1, pos2, color, true);
}

/// <summary>
/// スクリーン上に線を描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
static void DrawLine(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color) {
	//Primitive2D::Instance()->DrawLine(pos1, pos2, color);
}

#pragma region DrawGraph

/// <summary>
/// スクリーン上に画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
static void DrawGraph(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
}

/// <summary>
/// スクリーン上に画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
static void DrawGraph(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, false);
}

/// <summary>
/// 3D空間上に画像付きの板ポリを描画する
/// まだZ座標対応してないので斜めポリゴンは無理です。
/// 第一引数で指定したZ座標の値が適応されます。
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
static void DrawGraph3D(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, true);
}

/// <summary>
/// スクリーン上にアルファ値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="alpha">アルファ値</param>
static void DrawGraphEffAlpha(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha });
}

/// <summary>
/// スクリーン上にアルファ値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="alpha">アルファ値</param>
static void DrawGraphEffAlpha(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, false);
}

/// <summary>
/// 3D空間上にアルファ値を指定して画像付きの板ポリを描画する
/// まだZ座標対応してないので斜めポリゴンは無理です。
/// 第一引数で指定したZ座標の値が適応されます。
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="alpha">アルファ値</param>
static void DrawGraph3DEffAlpha(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, true);
}

/// <summary>
/// スクリーン上に色影響値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="color">影響させる色</param>
static void DrawGraphEffColor(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color);
}

/// <summary>
/// スクリーン上に色影響値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="color">影響させる色</param>
static void DrawGraphEffColor(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color, false);
}

/// <summary>
/// 3D空間上に色影響値を指定して画像付きの板ポリを描画する
/// まだZ座標対応してないので斜めポリゴンは無理です。
/// 第一引数で指定したZ座標の値が適応されます。
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="color">影響させる色</param>
static void DrawGraph3DEffColor(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraph(pos1, pos2, TextureNumber, color, true);
}

#pragma region DrawGraphBack

/// <summary>
/// スクリーン上に画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
static void DrawGraphBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
}

/// <summary>
/// スクリーン上に画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
static void DrawGraphBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f }, false);
}

/// <summary>
/// スクリーン上にアルファ値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="alpha">アルファ値</param>
static void DrawGraphEffAlphaBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha });
}

/// <summary>
/// スクリーン上にアルファ値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="alpha">アルファ値</param>
static void DrawGraphEffAlphaBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, float alpha) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, XMFLOAT4{ 1.0f,1.0f,1.0f,alpha }, false);
}

/// <summary>
/// スクリーン上に色影響値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="color">影響させる色</param>
static void DrawGraphEffColorBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, color);
}

/// <summary>
/// スクリーン上に色影響値を指定して画像付きの板ポリを描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="TextureNumber">画像ハンドル</param>
/// <param name="color">影響させる色</param>
static void DrawGraphEffColorBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color) {
	Primitive2D::Instance()->DrawGraphBack(pos1, pos2, TextureNumber, color, false);
}
#pragma endregion
#pragma endregion

#pragma region 3D
/// <summary>
/// 3D空間上に立方体を描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, XMFLOAT3(1, 1, 1), XMFLOAT3(0,0,0), color, Lighting);
}

/// <summary>
/// 3D空間上に立方体を描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawCubeRotate(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, XMFLOAT3(1, 1, 1), rotate, color, Lighting);
}


/// <summary>
/// 3D空間上に立方体を描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawCubeScaleRotate(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawCube(pos1, pos2, scale, rotate, color, Lighting);
}

/// <summary>
/// 3D空間上に線分を描画する
/// </summary>
/// <param name="pos1">始点となる座標１</param>
/// <param name="pos2">終点となる座標２</param>
/// <param name="color">色</param>
static void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color) {
	Primitive3D::Instance()->DrawLine(pos1, pos2, color);
}

/// <summary>
/// 3D空間上にグリッド線を描画する
/// </summary>
/// <param name="pos1">基準となる頂点座標１</param>
/// <param name="pos2">基準となる頂点座標２</param>
/// <param name="interval">間隔</param>
/// <param name="color">色</param>
static void DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval, XMFLOAT4 color) {
	Primitive3D::Instance()->DrawGrid(pos1, pos2, interval, color);
}

/// <summary>
/// 3D空間上に四面体を描画する
/// </summary>
/// <param name="pos1">頂点座標１</param>
/// <param name="pos2">頂点座標２</param>
/// <param name="pos3">頂点座標３</param>
/// <param name="pos4">頂点座標４</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawTetrahedron(pos1, pos2, pos3, pos4, color, Lighting);
}

/// <summary>
/// 3D空間上に球で範囲を指定して正八面体を描画する
/// </summary>
/// <param name="pos">中心座標</param>
/// <param name="Radius">半径</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawOctahedron(pos, Radius, color, Lighting);
}

/// <summary>
/// 3D空間上に球で範囲を指定して正二十面体を描画する
/// </summary>
/// <param name="pos">中心座標</param>
/// <param name="Radius">半径</param>
/// <param name="color">色</param>
/// <param name="Lighting">ライティングを有効にするか</param>
static void DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false) {
	Primitive3D::Instance()->DrawIcosahedron(pos, Radius, color, Lighting);
}
#pragma endregion