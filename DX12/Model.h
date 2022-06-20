#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include"Input.h"
#include "TexManager.h"
#include <string>
#include <list>
#include <array>
#include <memory>
#include "Camera.h"
#include "PipelineManager.h"
#include <fstream>
#include <sstream>
using namespace std;
#include "fbxsdk.h"
#include <unordered_map>

struct VertexPosUv {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//頂点データ構造体
struct VertexPos {
	XMFLOAT3 pos;
};

struct Material {
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFilename;

	Material() {
		ambient = { 1.0f, 1.0f, 1.0f };
		diffuse = { 1.0f, 1.0f, 1.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 0.3f;
	}

	UINT texNumber = 0; //テクスチャ番号
	UINT BumpTexNumber = 0; //バンプマッピング用テクスチャ番号
	UINT HeightTexNumber = 0; //視差マッピング用テクスチャ番号
	ComPtr<ID3D12Resource> constBuffB1;
};

struct FBXNode {
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	FBXNode *parent = nullptr;
};

//ボーンインデックスの最大数
static const int MAX_BONE_INDICES = 4;
struct Vertex {
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,0,0 }; //法線ベクトル
	XMFLOAT2 uv = { 0,0 };
	signed short boneIndex[MAX_BONE_INDICES] = { -1 };
	signed short boneIndexB[MAX_BONE_INDICES] = { -1 };
	float boneWeight[MAX_BONE_INDICES] = { 0 };
	float boneWeightB[MAX_BONE_INDICES] = { 0 };
};

struct Model {
	friend class FbxLoader;
public:
	string fileName; //ファイル名
	int modelID;

	Model *parent = nullptr;

public: //定数

	std::string tag = "None";
	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Material material;

	int _indices = 0; //頂点数
	bool isMaterial = false; //マテリアルを適応するか
	XMFLOAT3 scale = { 1,1,1 };

	//FBX用
	//モデル名
	std::string name;
	//ノード配列
	std::vector<FBXNode> nodes;
	//メッシュを持つノード
	FBXNode *meshNode = nullptr;
	//描画
	void Draw();
	//モデルの変形行列取得
	const XMMATRIX &GetModelTransform() { return meshNode->globalTransform; }

	//ボーン構造体
	struct Bone {
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster *fbxCluster;
		//コンストラクタ
		Bone(const std::string &name) {
			this->name = name;
		}
	};
	//ボーン配列
	std::vector<Bone> bones;
	//getter
	std::vector<Bone> &GetBones() { return bones; }
	//FBXシーン
	FbxScene *fbxScene = nullptr;
	//getter
	FbxScene *GetFbxScene() { return fbxScene; }
	~Model();

	//スムージング
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns></returns>
	inline size_t GetVertexCount() { return vertices.size(); }
	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();
};