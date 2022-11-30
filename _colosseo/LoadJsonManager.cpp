#include "LoadJsonManager.h"
#include <fstream>

void LoadJsonManager::LoadJson(const char* _FileName) {
	std::ifstream File;
	// �t�@�C�����J��
	File.open(_FileName);
	// �t�@�C���I�[�v�����s�Ȃ�assert
	if (File.fail()) {
		assert(0);
	}
	// Json�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;
	// ��
	File >> deserialized;
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());
	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get <std::string>();
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);
	// 
	//ObjectsData.emplace_back();
	//LevelData* LevelData = &ObjectsData[ObjectsData.size() - 1];

	LevelsData.emplace_back();
	LevelData* Level = &LevelsData.back();
	Level->FileName = _FileName;
	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0) {
			Level->ObjectsData.emplace_back();
			ObjectData& objData = Level->ObjectsData.back();
			if (object.contains("file_name")) {
				objData.FileName = object["file_name"];
			}
			if (object.contains("Shader_Name")) {
				objData.ShaderName = object["Shader_Name"];
			}
			if (object.contains("name")) {
				objData.Name = object["name"];
			}
			if (object.contains("transform")) {
				nlohmann::json transform = object["transform"];
				objData.Position.x = float(transform["translation"][1]);
				objData.Position.y = float(transform["translation"][2]);
				objData.Position.z = -float(transform["translation"][0]);

				objData.Rotate.x = -float(transform["rotation"][1]);
				objData.Rotate.y = -float(transform["rotation"][2]);
				objData.Rotate.z = float(transform["rotation"][0]);

				objData.Scale.x = float(transform["scaling"][1]);
				objData.Scale.y = float(transform["scaling"][2]);
				objData.Scale.z = float(transform["scaling"][0]);
			}
		}
	}
}

std::vector<Object3d*> LoadJsonManager::LoadScene(const char* _FileName)
{
	std::vector<Object3d*> objects;
	for (auto& data : LevelsData) {
		if (_FileName == data.FileName) {
			for (auto& obj : data.ObjectsData) {
				int aA = 0;
				int ModelHandle = LoadModelOBJ(obj.FileName, obj.Name);
				Object3d* hoge = DirectX3dObject::CreateObject(GetModelData(ModelHandle),
					XMFLOAT3(0, 0, 0), Toenum_name(obj.ShaderName));

				hoge->position = obj.Position;
				hoge->rotation = obj.Rotate;
				hoge->scale = obj.Scale;
				
				objects.emplace_back(hoge);
			}
		}
	}
	return objects;
}
