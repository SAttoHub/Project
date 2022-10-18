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
// Collider �g����
// �ȉ��� class �ɒǋL
//
//	public:
//		GameBaseCollider *collider = nullptr;
//		virtual ~�N���X��();
//		void SetCollider(GameBaseCollider *collider);
//		void ColliderUpdate();
//
//-------------------------------------------------------------------------------------------------------------

// �ȉ� class �̉����Ƃ��� Class ���g�p

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
//		collider->SetBasisPosition(&��_���W);
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
// Collider ���W
//	
//	ColliderProp ���p�����邱�Ƃɂ����Hit��̃I�u�W�F�N�g���擾�ł���
//	class Class : public ColliderProp {
//	}
//
//	����Ȋ����Ŏg��
//	if(CheckCollision(collider, Tag)){
//		prop��Hit��̃I�u�W�F�N�g( �N���X�� : Class )���i�[
//		Class* prop = GetHitObjectPtr<Class*>();
//	}
//-------------------------------------------------------------------------------------------------------------

class GCM : public Singleton<GCM> {
	friend class Singleton<GCM>;

public:
	/// <summary>
	/// �R���C�_�[�����X�g�ɒǉ�����
	/// </summary>
	/// <param name="collider">�ǉ�����R���C�_�[�̃|�C���^</param>
	inline void AddCollider(GameBaseCollider *collider) {
		colliders.push_front(collider);
	}
	/// <summary>
	/// �R���C�_�[�����X�g����폜����
	/// </summary>
	/// <param name="collider">�폜����R���C�_�[�̃|�C���^</param>
	inline void RemoveCollider(GameBaseCollider *collider) {
		colliders.remove(collider);
	}
	
	/// <summary>
	/// �w�肵���^�O�����R���C�_�[�ƏՓ˔�����s��
	/// </summary>
	/// <param name="col">�R���C�_�[</param>
	/// <param name="Tag">�Փ˔�����s���Փː�̃^�O</param>
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
	/// ���O�ɏՓ˂����Ώۂ̃R���C�_�[�̃|�C���^��Ԃ�
	/// </summary>
	/// <returns></returns>
	GameBaseCollider *GetHitCollider() {
		return colB;
	}
};

/// <summary>
/// �w�肵���^�O�����R���C�_�[�ƏՓ˔�����s��
/// </summary>
/// <param name="col">�R���C�_�[</param>
/// <param name="Tag">�Փ˔�����s���Փː�̃^�O</param>
/// <returns></returns>
static bool CheckCollision(GameBaseCollider *col, std::string Tag)
{
	return GCM::Instance()->CheckCollision(col, Tag);
}

/// <summary>
/// ���O�ɏՓ˂����I�u�W�F�N�g�̃|�C���^��Ԃ�
/// </summary>
/// <typeparam name="T">�Փ˂����I�u�W�F�N�g�̃|�C���^�^</typeparam>
/// <returns></returns>
template <class T>
static T GetHitObjectPtr() {
	return dynamic_cast<T>(GCM::Instance()->GetHitCollider()->Base);
}

	//class Object_1 : public ColliderProp {
	//	GameBaseCollider *col_obj1; // "OBJ_1"�^�O��t����collider
	//public:
	//	Object_1();
	//	void Death() {}
	//};

	//class Object_2 {
	//	GameBaseCollider *col_obj2; // ������collider
	//	Object_2();
	//	void Test(){
	//		if (CheckCollision(col_obj2, "OBJ_1")) {
	//			// �Փ˂���collider�̕t���Ă��� Object_1�N���X���擾����
	//			Object_1 *obj1 = GetHitObjectPtr<Object_1 *>();
	//			obj1->Death();
	//		}
	//	}
	//};
