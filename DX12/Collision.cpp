#include "Collision.h"
#include "Collision2.h"


XMVECTOR Collision::forwardVec;
XMFLOAT3 Collision::forwardVecF;
bool Collision::headshot;
std::vector<Triangle> Collision::BOSS1_Body_T;
std::vector<Triangle> Collision::BOSS1_Core_T;

Collision::Collision() {
}



void Collision::Update(Object3d *player)
{
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(player->rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(player->rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(player->rotation.y));
	XMFLOAT3 forward = { 0,0,1 };
	forwardVec = XMLoadFloat3(&forward);
	forwardVec = XMVector3TransformCoord(forwardVec, matRot);
	forwardVec = XMVector3Normalize(forwardVec);
	XMStoreFloat3(&forwardVecF, forwardVec);
}

bool Collision::RayPlaneCol(XMVECTOR normal, float dis, float *distance, DirectX::XMVECTOR *inter)
{
	const float epsilon = 1.0e-5f;
	float d1 = XMVector3Dot(normal, forwardVec).m128_f32[0];
	if (d1 > -epsilon) {
		return false;
	}
	XMVECTOR start = XMLoadFloat3(&Camera::eye);
	float d2 = XMVector3Dot(normal, start).m128_f32[0];
	float t = (dis - d2) / d1;
	if (t < 0) {
		return false;
	}
	if (distance) {
		*distance = t;
	}
	if (inter) {
		*inter = start + t * forwardVec;
	}
	return true;
}

bool Collision::RaySphereCol(Object3d *player, Object3d *target, float target_r, float *distance, DirectX::XMVECTOR *inter)
{
	headshot = false;
	XMVECTOR start = XMLoadFloat3(&Camera::eye);

	XMVECTOR targetCenter = XMLoadFloat3(&target->position);

	XMVECTOR m = start - targetCenter;
	float b = XMVector3Dot(m, forwardVec).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - target_r * target_r;
	if (c > 0.0f && b > 0.0f) {
		return false;
	}
	float discr = b * b - c;
	if (discr < 0.0f) {
		return false;
	}
	float t = -b - sqrtf(discr);
	if (t < 0) t = 0.0f;

	if (distance) {
		*distance = t;
	}

	if (inter) {
		*inter = start + t * forwardVec;
	}

	return true;
}

bool Collision::RaySphereCol2(Object3d *player, XMFLOAT3 *target, float target_r, float *distance, DirectX::XMVECTOR *inter)
{
	headshot = false;
	XMVECTOR start = XMLoadFloat3(&Camera::eye);

	XMVECTOR targetCenter = XMLoadFloat3(target);

	XMVECTOR m = start - targetCenter;
	float b = XMVector3Dot(m, forwardVec).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - target_r * target_r;
	if (c > 0.0f && b > 0.0f) {
		return false;
	}
	float discr = b * b - c;
	if (discr < 0.0f) {
		return false;
	}
	float t = -b - sqrtf(discr);
	if (t < 0) t = 0.0f;

	if (distance) {
		*distance = t;
	}

	if (inter) {
		*inter = start + t * forwardVec;
	}

	return true;
}

bool Collision::RayCilinderCol(Object3d *player, Object3d *target, float Cilinderdistance, float target_r, float *distance, DirectX::XMFLOAT3 *inter)
{
	headshot = false;
	XMFLOAT3 CilinderPos1, CilinderPos2;
	XMFLOAT3 radian;
	radian.x = target->rotation.x * M_PI / 180.0f;
	radian.y = target->rotation.y * M_PI / 180.0f;
	radian.z = 0;

	CilinderPos1.x = Cilinderdistance * sin(radian.y) * cos(radian.x) + target->position.x;
	CilinderPos1.y = -Cilinderdistance * sin(radian.x) + target->position.y;
	CilinderPos1.z = Cilinderdistance * cos(radian.y) * cos(radian.x) + target->position.z;

	CilinderPos2.x = -Cilinderdistance * sin(radian.y) * cos(radian.x) + target->position.x;
	CilinderPos2.y = Cilinderdistance * sin(radian.x) + target->position.y;
	CilinderPos2.z = -Cilinderdistance * cos(radian.y) * cos(radian.x) + target->position.z;

	XMFLOAT3 Keep = CilinderPos2;

	float px = CilinderPos1.x - Camera::eye.x;
	float py = CilinderPos1.y - Camera::eye.y;
	float pz = CilinderPos1.z - Camera::eye.z;
	CilinderPos2.x = CilinderPos2.x - Camera::eye.x;
	CilinderPos2.y = CilinderPos2.y - Camera::eye.y;
	CilinderPos2.z = CilinderPos2.z - Camera::eye.z;



	float sx = CilinderPos2.x - px;
	float sy = CilinderPos2.y - py;
	float sz = CilinderPos2.z - pz;

	// äeéÌì‡êœíl
	float Dvv = forwardVecF.x * forwardVecF.x + forwardVecF.y * forwardVecF.y + forwardVecF.z * forwardVecF.z;
	float Dsv = sx * forwardVecF.x + sy * forwardVecF.y + sz * forwardVecF.z;
	float Dpv = px * forwardVecF.x + py * forwardVecF.y + pz * forwardVecF.z;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = target_r * target_r;

	if (Dss == 0.0f) {
		return false;
	}

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f) {
		return false;
	}

	float s = B * B - A * C;
	if (s < 0.0f) {
		return false;
	}

	s = sqrtf(s);

	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	inter->x = Camera::eye.x + a1 * forwardVecF.x;
	inter->y = Camera::eye.y + a1 * forwardVecF.y;
	inter->z = Camera::eye.z + a1 * forwardVecF.z;

	XMVECTOR inter2 = XMLoadFloat3(inter);
	XMVECTOR inter3 = XMLoadFloat3(inter);
	XMFLOAT3 inter4;

	float dist1 = 0, dist2 = 0;

	bool F1 = RaySphereCol2(player, &CilinderPos1, target_r, &dist1, &inter2);
	bool F2 = RaySphereCol2(player, &Keep, target_r, &dist2, &inter3);

	if (!F1 && !F2) {
		
	}
	else {
		if (dist1 < dist2) {
			XMStoreFloat3(&inter4, inter2);
		}
		else {
			XMStoreFloat3(&inter4, inter3);
			headshot = true;
		}
		inter->x = inter4.x;
		inter->y = inter4.y;
		inter->z = inter4.z;
		return true;
	}
	XMFLOAT3 pos1, pos2;
	if (CilinderPos1.x > Keep.x) {
		pos1.x = CilinderPos1.x;
		pos2.x = Keep.x;
	}
	else {
		pos1.x = Keep.x;
		pos2.x = CilinderPos1.x;
	}
	if (CilinderPos1.y > Keep.y) {
		pos1.y = CilinderPos1.y;
		pos2.y = Keep.y;
	}
	else {
		pos1.y = Keep.y;
		pos2.y = CilinderPos1.y;
	}
	if (CilinderPos1.z > Keep.z) {
		pos1.z = CilinderPos1.z;
		pos2.z = Keep.z;
	}
	else {
		pos1.z = Keep.z;
		pos2.z = CilinderPos1.z;
	}


	if (pos1.x > inter->x && pos2.x < inter->x) {
		return true;
	}
	if (pos1.y > inter->y && pos2.y < inter->y) {
		return true;
	}
	if (pos1.z > inter->z && pos2.z < inter->z) {
		return true;
	}


	return false;
}

bool Collision::SphereCol(XMFLOAT3 playerPos, float playerR, XMFLOAT3 targetPos, float targetR)
{
	float X = (targetPos.x - playerPos.x) * (targetPos.x - playerPos.x);
	float Y = (targetPos.y - playerPos.y) * (targetPos.y - playerPos.y);
	float Z = (targetPos.z - playerPos.z) * (targetPos.z - playerPos.z);
	float R = (playerR + targetR) * (playerR + targetR);
	if (X + Y + Z <= R) {
		return true;
	}
	return false;
}

bool Collision::MeshSphereCol(Object3d *obj, const Sphere &sphere, DirectX::XMVECTOR *inter) {
	std::vector<Triangle> triangles;
	size_t triangleNum = obj->model->_indices / 3;
	triangles.resize(triangles.size() + triangleNum);
	for (int i = 0; i < triangleNum; i++) {
		int idx0 = obj->model->indices[i * 3 + 0];
		int idx1 = obj->model->indices[i * 3 + 1];
		int idx2 = obj->model->indices[i * 3 + 2];

		triangles[i].p0 = {
				obj->model->vertices[idx0].pos.x,
				obj->model->vertices[idx0].pos.y,
				obj->model->vertices[idx0].pos.z,
				1 };

		triangles[i].p1 = {
			obj->model->vertices[idx1].pos.x,
			obj->model->vertices[idx1].pos.y,
			obj->model->vertices[idx1].pos.z,
			1 };

		triangles[i].p2 = {
			obj->model->vertices[idx2].pos.x,
			obj->model->vertices[idx2].pos.y,
			obj->model->vertices[idx2].pos.z,
			1 };

		triangles[i].ComputeNormal();
	}
	XMMATRIX invMatWorld = XMMatrixInverse(nullptr, obj->matWorld);

	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle &triangle = *it;

		if (Collision2::CheckSphere2Triangle(localSphere, triangle, inter)) {
			if (inter) {
				const XMMATRIX &matWorld = obj->matWorld;

				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}

	return false;
}

XMFLOAT3 Collision::CapsuleSpherePos(Object3d *target, float Cilinderdistance)
{
	headshot = false;
	XMFLOAT3 CilinderPos1, CilinderPos2;
	XMFLOAT3 radian;
	radian.x = target->rotation.x * M_PI / 180.0f;
	radian.y = target->rotation.y * M_PI / 180.0f;
	radian.z = 0;

	CilinderPos1.x = Cilinderdistance * sin(radian.y) * cos(radian.x) + target->position.x;
	CilinderPos1.y = -Cilinderdistance * sin(radian.x) + target->position.y;
	CilinderPos1.z = Cilinderdistance * cos(radian.y) * cos(radian.x) + target->position.z;

	CilinderPos2.x = -Cilinderdistance * sin(radian.y) * cos(radian.x) + target->position.x;
	CilinderPos2.y = Cilinderdistance * sin(radian.x) + target->position.y;
	CilinderPos2.z = -Cilinderdistance * cos(radian.y) * cos(radian.x) + target->position.z;
	return CilinderPos1; //ç°âÒÇÕ1ÇµÇ©égÇÌÇ»Ç¢
}

void Collision::LoadMesh(Object3d *obj, int Type) {
	size_t triangleNum = obj->model->_indices / 3;
	if(Type == BOSS1_Body){
	BOSS1_Body_T.resize(BOSS1_Body_T.size() + triangleNum);
	for (int i = 0; i < triangleNum; i++) {
		int idx0 = obj->model->indices[i * 3 + 0];
		int idx1 = obj->model->indices[i * 3 + 1];
		int idx2 = obj->model->indices[i * 3 + 2];

		BOSS1_Body_T[i].p0 = {
				obj->model->vertices[idx0].pos.x,
				obj->model->vertices[idx0].pos.y,
				obj->model->vertices[idx0].pos.z,
				1 };

		BOSS1_Body_T[i].p1 = {
			obj->model->vertices[idx1].pos.x,
			obj->model->vertices[idx1].pos.y,
			obj->model->vertices[idx1].pos.z,
			1 };

		BOSS1_Body_T[i].p2 = {
			obj->model->vertices[idx2].pos.x,
			obj->model->vertices[idx2].pos.y,
			obj->model->vertices[idx2].pos.z,
			1 };
		BOSS1_Body_T[i].ComputeNormal();
		}
	}
	else if (Type == BOSS1_Core) {
		BOSS1_Core_T.resize(BOSS1_Core_T.size() + triangleNum);
		for (int i = 0; i < triangleNum; i++) {
			int idx0 = obj->model->indices[i * 3 + 0];
			int idx1 = obj->model->indices[i * 3 + 1];
			int idx2 = obj->model->indices[i * 3 + 2];

			BOSS1_Core_T[i].p0 = {
					obj->model->vertices[idx0].pos.x,
					obj->model->vertices[idx0].pos.y,
					obj->model->vertices[idx0].pos.z,
					1 };

			BOSS1_Core_T[i].p1 = {
				obj->model->vertices[idx1].pos.x,
				obj->model->vertices[idx1].pos.y,
				obj->model->vertices[idx1].pos.z,
				1 };

			BOSS1_Core_T[i].p2 = {
				obj->model->vertices[idx2].pos.x,
				obj->model->vertices[idx2].pos.y,
				obj->model->vertices[idx2].pos.z,
				1 };
			BOSS1_Core_T[i].ComputeNormal();
		}
	}
}

bool Collision::GameMeshSphereCol(std::vector<Triangle> Triangles, Object3d *obj, const Sphere &sphere, DirectX::XMVECTOR *inter)
{
	XMMATRIX invMatWorld = XMMatrixInverse(nullptr, obj->matWorld);

	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator it = Triangles.cbegin();

	for (; it != Triangles.cend(); ++it) {
		const Triangle &triangle = *it;

		if (Collision2::CheckSphere2Triangle(localSphere, triangle, inter)) {
			if (inter) {
				const XMMATRIX &matWorld = obj->matWorld;

				*inter = XMVector3Transform(*inter, matWorld);
			}
			
			return true;
		}
	}

	return false;
}
