#include "Model.h"

void Model::Draw()
{
}

Model::~Model() {
    //FBX�V�[���̉��
    //fbxScene->Destroy();
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) {
    smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals() {
    auto itr = smoothData.begin();
    for (; itr != smoothData.end(); ++itr) {
        //�e���_�p�̋��ʒ��_�R���N�V����
        std::vector<unsigned short> &v = itr->second;
        //�S���_�̖@���𕽋ς���
        XMVECTOR normal = {};
        for (unsigned short index : v) {
            normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
        }
        normal = XMVector3Normalize(normal / (float)v.size());
        //���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
        for (unsigned short index : v) {
            vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
        }
    }
}
