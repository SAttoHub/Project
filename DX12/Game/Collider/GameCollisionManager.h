#pragma once
#include <forward_list>
#include <string>
#include "..\..\Engine\Common\DirectXMath\DirectXMathExpansion.h"
#include "GameBaseCollider.h"
#include "..\..\Collision2.h"
#include "..\..\CollisionPrimitive.h"
#include "..\..\Singleton.h"
#include "GameCircleCollider.h"
#include "GameBoxCollider.h"
#include "GamePointCollider.h"

//-------------------------------------------------------------------------------------------------------------
// Collider 使い方
// 以下を class に追記
//
//	public:
//		GameBaseCollider *collider = nullptr;
//		virtual ~クラス名();
//		void SetCollider(GameBaseCollider *collider);
//		void ColliderUpdate();
//
//-------------------------------------------------------------------------------------------------------------

// 以下 class の仮名として Class を使用

//-------------------------------------------------------------------------------------------------------------
//	Class::~Class()
//	{
//		if (collider) {
// 			GCM::Instance()->RemoveCollider(collider);
// 			delete collider;
// 		}
//	}
//-------------------------------------------------------------------------------------------------------------
//	void Class::SetCollider(GameBaseCollider *collider)
//	{
//		collider->SetBasisPosition(&基点座標);
//		this->collider = collider;
//		GCM::Instance()->AddCollider(collider);
//	}
//-------------------------------------------------------------------------------------------------------------
//	void Class::ColliderUpdate()
//	{
//		if (collider) {
//			collider->Update();
//		}
//	}
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// Collider 発展
//	
//	ColliderProp を継承することによってHit先のオブジェクトを取得できる
//	class Class : public ColliderProp {
//	}
//
//	こんな感じで使う
//	if(CheckCollision(collider, Tag)){
//		propにHit先のオブジェクト( クラス名 : Class )を格納
//		Class* prop = GetHitObjectPtr<Class*>();
//	}
//-------------------------------------------------------------------------------------------------------------

class GCM : public Singleton<GCM> {
	friend class Singleton<GCM>;

public:
	/// <summary>
	/// コライダーをリストに追加する
	/// </summary>
	/// <param name="collider">追加するコライダーのポインタ</param>
	inline void AddCollider(GameBaseCollider *collider) {
		colliders.push_front(collider);
	}
	/// <summary>
	/// コライダーをリストから削除する
	/// </summary>
	/// <param name="collider">削除するコライダーのポインタ</param>
	inline void RemoveCollider(GameBaseCollider *collider) {
		colliders.remove(collider);
	}
	
	/// <summary>
	/// 指定したタグを持つコライダーと衝突判定を行う
	/// </summary>
	/// <param name="col">コライダー</param>
	/// <param name="Tag">衝突判定を行う衝突先のタグ</param>
	/// <returns></returns>
	bool CheckCollision(GameBaseCollider* col, std::string Tag);

private:
	GCM() = default;
	GCM(const GCM &) = delete;
	~GCM() = default;
	GCM &operator=(const GCM &) = delete;

	std::forward_list<GameBaseCollider * > colliders;
	GameBaseCollider *colB;
	std::forward_list<GameBaseCollider *>::iterator itB;

public:
	/// <summary>
	/// 直前に衝突した対象のコライダーのポインタを返す
	/// </summary>
	/// <returns></returns>
	GameBaseCollider *GetHitCollider() {
		return colB;
	}
};

/// <summary>
/// 指定したタグを持つコライダーと衝突判定を行う
/// </summary>
/// <param name="col">コライダー</param>
/// <param name="Tag">衝突判定を行う衝突先のタグ</param>
/// <returns></returns>
static bool CheckCollision(GameBaseCollider *col, std::string Tag)
{
	return GCM::Instance()->CheckCollision(col, Tag);
}

/// <summary>
/// 直前に衝突したオブジェクトのポインタを返す
/// </summary>
/// <typeparam name="T">衝突したオブジェクトのポインタ型</typeparam>
/// <returns></returns>
template <class T>
static T GetHitObjectPtr() {
	return dynamic_cast<T>(GCM::Instance()->GetHitCollider()->Base);
}

	//class Object_1 : public ColliderProp {
	//	GameBaseCollider *col_obj1; // "OBJ_1"タグを付けたcollider
	//public:
	//	Object_1();
	//	void Death() {}
	//};

	//class Object_2 {
	//	GameBaseCollider *col_obj2; // 自分のcollider
	//	Object_2();
	//	void Test(){
	//		if (CheckCollision(col_obj2, "OBJ_1")) {
	//			// 衝突したcolliderの付いている Object_1クラスを取得する
	//			Object_1 *obj1 = GetHitObjectPtr<Object_1 *>();
	//			obj1->Death();
	//		}
	//	}
	//};
