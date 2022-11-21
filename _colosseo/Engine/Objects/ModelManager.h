#pragma once
#include "Model.h"
#include "Loader\FbxLoader.h"
#include "..\..\Singleton.h"

class ModelManager : public Singleton<ModelManager> {
	friend class Singleton<ModelManager>;
private:

public:
	ModelManager() {
		models.reserve(516);
	}
	~ModelManager() {}
	vector<Model> models;
	int NowLoadNum = 0;
	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <param name="tag">���f���ɕt����^�O</param>
	/// <param name="Smoothing">�X���[�W���O�t���O</param>
	/// <returns></returns>
	int LoadModelOBJ(const std::string &modelname, std::string tag = "None", bool Smoothing = false);
	int LoadModelFBX(const string &modelName);

	Model *GetModelData(int modelNum);
private:
	void LoadMaterial(Model *model, const std::string &directoryPath, const std::string &filename);

	//���Ƀ��[�h�ς݂̏ꍇ�͂��̃��f����ID��Ԃ��B���[�h����Ă��Ȃ������ꍇ�� -1 ��Ԃ�
	int AlreadyLoadModel(const string &Name);
};

static int LoadModelOBJ(const std::string &modelname, std::string tag = "None", bool Smoothing = false) {
	return ModelManager::Instance()->LoadModelOBJ(modelname, tag, Smoothing);
}
static int LoadModelFBX(const string &modelName) {
	return ModelManager::Instance()->LoadModelFBX(modelName);
}
static Model *GetModelData(int modelNum) {
	return ModelManager::Instance()->GetModelData(modelNum);
}

