#pragma once
#include "3DObject.h"
#include "CollisionPrimitive.h"

enum DIRECTION {
	UP,
	DOWN,
	RIGHT,
	LEFT
};
//�Q�[���p
enum MeshType {
	BOSS1_Body,
	BOSS1_Core
};

class Collision {
public:
	Collision();
	~Collision() {}

	static XMVECTOR forwardVec;
	static XMFLOAT3 forwardVecF;
	static bool headshot;

	static void Update(Object3d *player);

	static bool RayPlaneCol(XMVECTOR normal, float dis, float *distance, DirectX::XMVECTOR *inter);
	static bool RaySphereCol(Object3d *player, Object3d *target, float target_r, float *distance, DirectX::XMVECTOR *inter);
	static bool RaySphereCol2(Object3d *player, XMFLOAT3 *target, float target_r, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
	static bool RayCilinderCol(Object3d *player, Object3d *target,float Cilinderdistance, float target_r, float *distance = nullptr, DirectX::XMFLOAT3 *inter = nullptr);

	static bool SphereCol(XMFLOAT3 playerPos, float playerR, XMFLOAT3 targetPos, float targetR);

	static bool MeshSphereCol(Object3d *obj, const Sphere &sphere, DirectX::XMVECTOR *inter = nullptr); //�N�\�d����

	//�J�v�Z���̋��̍��W�����߂�
	static XMFLOAT3 CapsuleSpherePos(Object3d *target, float Cilinderdistance);

	//�ꎞ�I�ȃ��b�V���̕ۑ�(�Q�[���p)
	static std::vector<Triangle> BOSS1_Body_T;
	static std::vector<Triangle> BOSS1_Core_T;
	static void LoadMesh(Object3d *obj, int Type);
	static bool GameMeshSphereCol(std::vector<Triangle> Triangles, Object3d* obj, const Sphere &sphere, DirectX::XMVECTOR *inter = nullptr);
};

