#pragma once
#include "fbxsdk.h"
#include <d3d12.h>
#include <d3dx12.h>
#include "DirectXBase.h"
#include <string>
#include <map>
#include <forward_list>
#include "Model.h"

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader *GetInstance();
	void Initialize();
	void Finalize();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader &obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader &obj) = delete;

	FbxManager *fbxManager = nullptr;
	FbxImporter *fbxImporter = nullptr;

private:
	using string = std::string;
	class BoneAffectInfo
	{
	public:
		signed short index;
		float weight;
	};

public:
	//モデルのルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
	//モデルのロード
	Model LoadModelFromFile(const string &modelName);
	/// <summary>
	/// 再帰的にノードを解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(Model *model, FbxNode *fbxNode, FBXNode *parent = nullptr, std::string OyaName = "");
	/// <summary>
	/// メッシュの読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(Model *model, FbxNode *fbxNode);
	//頂点座標の読み取り
	void ParseMeshVertices(Model *model, FbxMesh *fbxMesh, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable);
	//面情報の読み取り
	void ParseMeshFaces(Model *model, FbxMesh *fbxMesh);
	//マテリアルの読み取り
	void ParseMaterial(Model *model, FbxNode *fbxNode);
	//スキニングの情報読み取り
	void ParseSkin(Model *model, FbxMesh *fbxMesh, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable);
	void LoadConvertFBXBone(FbxCluster *Cluster, int BoneIndex, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable);
	void SetBoneAffectToVertex(std::map<int, std::forward_list<BoneAffectInfo>> &Info, Vertex *VertexPtr, int Index);
	bool Test(Vertex *VertexPtr);
	//テクスチャの読み込み
	void LoadTexture(Model *model, const std::string &fullpath);
	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string &path);

	void GetMesh(FbxNode *Node, std::vector<FbxMesh *> *Mesh);

	static void ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src);

	// FBXシーンのみの読み込み
	FbxScene* LoadFBXScene(const string &modelName);
};