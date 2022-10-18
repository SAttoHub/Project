#pragma once
#include "Model.h"
#include "..\..\CollisionInfo.h"
#include "..\Light\LightGroup.h"
#include "ModelManager.h"
#include "..\..\Engine\Common\Containers\MyFList.h"

class BaseCollider;

//定数バッファ用構造体
struct ConstBufferData {
	XMFLOAT4 color; //色(RGBA)
	XMMATRIX mat; //3D変換行列
};

struct ConstBufferDataB0 {
	XMMATRIX view;
	XMMATRIX viewproj; //ビュープロジェクション行列
	XMMATRIX viewproj2; //ビュープロジェクション行列2
	XMMATRIX world; //ワールド行列
	XMFLOAT3 cameraPos; //カメラ座標（ワールド座標）
	float pad1; // パディング
	XMFLOAT4 color;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient; // アンビエント係数
	float pad1; // パディング
	XMFLOAT3 diffuse; // ディフューズ係数
	float pad2; // パディング
	XMFLOAT3 specular; // スペキュラー係数
	float alpha;	// アルファ
};

//定数バッファ用構造体
struct ConstBufferDataOther {
	UINT time; //色(RGBA)
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

struct ConstBufferDataShadow {
	XMMATRIX Light_view; // ビュー行列
	XMMATRIX Light_viewproj;
	XMFLOAT3 Light_Pos;
};

//3Dオブジェクト型
struct Object3d {
	ComPtr<ID3D12Resource> constBuffB0;
	//アフィン変換情報
	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldrotation = { -1,0,0 };
	XMFLOAT3 oldscale = { -1,1,1 };
	XMFLOAT3 oldposition = { -1,0,0 };
	XMMATRIX matWorld;
	Object3d *parent = nullptr;
	UINT shaderNumber = 0;
	bool isBillboard = false;//ビルボード
	bool DrawFlag = true; //DrawFlag
	bool alwaysUpdate = false; // 常にマッピングし直すか


	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Material material;
	int _indices = 0; //頂点数
	bool isMaterial = false; //マテリアルを適応するか
	ComPtr<ID3D12Resource> vertBuff; //頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> indexBuff;

	//FBXシーン
	//FbxScene *fbxScene;
	Model *model = nullptr; //モデル情報
	//FbxScene *fbxScene = nullptr;
	//std::vector<Model::Bone> bones;

public: //スキニング
	//1メッシュが持てるボーンの最大数
	static const int MAX_BONES = 61;
	//定数バッファ用構造体(スキニング)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション速度倍率
	float flameSpeed = 1.0f;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中かどうか
	bool isPlay = false;
	//アニメーションをループさせるかどうか
	bool isLoop = false;
	//アニメーション終了時にそこで止めるかどうか
	bool isEndStop = false;
	//再生中のアニメーション番号
	int PlayAnimationNumber = 0;
	/// <summary>
	/// アニメーション開始(最初から)
	/// </summary>
	/// <param name="AnimationNumber">再生するアニメーションの番号 (未入力は 0番 : -1 で現在セット中のアニメーション )</param>
	/// <param name="Loop">ループさせるかどうか (未入力は false )</param>
	void PlayAnimation(int AnimationNumber = 0, bool Loop = false);
	/// <summary>
	/// アニメーションの一時停止
	/// </summary>
	void StopAnimation();
	/// <summary>
	/// アニメーションの再生(途中から)　※アニメーションがセットされていない場合は例外で落ちる(多分)
	/// </summary>
	void StartAnimation();
	/// <summary>
	/// アニメーションのリセット
	/// </summary>
	void ResetAnimetion();
	/// <summary>
	/// アニメーション速度を変更
	/// </summary>
	/// <param name="Speed">速度</param>
	void ChangeAnimationSpeed(float Speed);

public:
	BaseCollider *collider = nullptr;
	Object3d *ptr = this;
	Object3d() = default;
	virtual ~Object3d();
	void SetCollider(BaseCollider *collider);
	void ColliderUpdate();
	virtual void OnCollision(const CollisionInfo &info) {}

public:
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX &GetMatWorld() { return matWorld; }

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffTime;
	UINT Timer = 0;

	ComPtr<ID3D12Resource> constBuffShadow;
};

class DirectX3dObject {
private:
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト

	//定数バッファビューの生成
	const int constantBufferNum = 1024; //定数バッファの最大数

	//定数バッファへのデータの転送
	void TransConstBuffer();
public:
	static int OldShaderLoad;//前回セットしたシェーダー

	//初期化処理
	void DirectX3DObjectReset(Window *Win);
	//描画コマンド
	void DrawReset() {
		for (int i = 0; i < (int)object3ds.size(); i++) {
			//[i].DrawFlag = false;
			object3ds[i]->DrawFlag = false;
			//forward_list_GetPtr(&object3ds, i)->DrawFlag = false;
		};
	}

	void Draw();


	static My_F_List<Object3d> object3ds;

	DirectX3dObject() {
		//object3ds.reserve(constantBufferNum); //定数バッファの最大数分だけ先にメモリを確保しておく
	}

	static void UnionObject3d(Object3d *oya, Object3d *kodomo) {
		kodomo->parent = oya;
	};

	static void UpdateObject3d(Object3d *object, XMMATRIX &matView, XMMATRIX &matProjection);

	/// <summary>
	/// オブジェクトの作成
	/// </summary>
	/// <param name="model">使用するモデルデータ</param>
	/// <param name="pos">生成位置</param>
	/// <param name="ShaderNum">使用するシェーダー</param>
	/// <returns></returns>
	static Object3d *CreateObject(Model *model, XMFLOAT3 pos, UINT ShaderNum);

	static void AllObjectUpdate();

	static void DeleteObject(Object3d *object);

	// ライト
	static LightGroup *lightGroup;
	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lightGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup *lightGroup) {
		DirectX3dObject::lightGroup = lightGroup;
	}

	//void DrawCube3D();
};



//オブジェクト初期化処理
void InitalizeObject3d(Object3d *object, int index);

/// <summary>
/// オブジェクトの描画
/// </summary>
/// <param name="object">使用するオブジェクトデータ</param>
/// <returns></returns>
void Drawobject3d(Object3d *object);
void DepthDrawobject3d(Object3d *object);
void ShadowDepthDrawobject3d(Object3d *object);
void DOFDepthDrawobject3d(Object3d *object);

/// <summary>
/// オブジェクトの描画
/// デフォルトライトを使用
/// </summary>
/// <param name="object">使用するオブジェクトデータ</param>
/// <returns></returns>
void Drawobject3dOfDefaultLight(Object3d *object);