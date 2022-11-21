#include "MeshCollider.h"
#include "Collision2.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(Model *model)
{
	triangles.clear();

	size_t triangleNum = model->_indices / 3;
	triangles.resize(triangles.size() + triangleNum);
	for (int i = 0; i < triangleNum; i++) {
		int idx0 = model->indices[i * 3 + 0];
		int idx1 = model->indices[i * 3 + 1];
		int idx2 = model->indices[i * 3 + 2];

		triangles[i].p0 = {
				model->vertices[idx0].pos.x,
				model->vertices[idx0].pos.y,
				model->vertices[idx0].pos.z,
				1 };

		triangles[i].p1 = {
			model->vertices[idx1].pos.x,
			model->vertices[idx1].pos.y,
			model->vertices[idx1].pos.z,
			1 };

		triangles[i].p2 = {
			model->vertices[idx2].pos.x,
			model->vertices[idx2].pos.y,
			model->vertices[idx2].pos.z,
			1 };

		triangles[i].ComputeNormal();
	}
}

void MeshCollider::Update()
{
	invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere &sphere, DirectX::XMVECTOR *inter)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle &triangle = *it;

		if (Collision2::CheckSphere2Triangle(localSphere, triangle, inter)) {
			if (inter) {
				const XMMATRIX &matWorld = GetObject3d()->GetMatWorld();

				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray &ray, float *distance, DirectX::XMVECTOR *inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle &triangle = *it;

		XMVECTOR tempInter;

		if (Collision2::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter)) {

			const XMMATRIX &matWorld = GetObject3d()->GetMatWorld();

			tempInter = XMVector3Transform(tempInter, matWorld);

			if (distance) {
				XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
			}

			if (inter) {
				*inter = tempInter;
			}

			return true;
		}
	}

	return false;
}
