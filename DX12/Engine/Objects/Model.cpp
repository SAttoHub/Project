#include "Model.h"

void Model::Draw()
{
}

Model::~Model() {
    //FBXシーンの解放
    //fbxScene->Destroy();
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) {
    smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals() {
    auto itr = smoothData.begin();
    for (; itr != smoothData.end(); ++itr) {
        //各頂点用の共通頂点コレクション
        std::vector<unsigned short> &v = itr->second;
        //全頂点の法線を平均する
        XMVECTOR normal = {};
        for (unsigned short index : v) {
            normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
        }
        normal = XMVector3Normalize(normal / (float)v.size());
        //共通法線を使用する全ての頂点データに書き込む
        for (unsigned short index : v) {
            vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
        }
    }
}
