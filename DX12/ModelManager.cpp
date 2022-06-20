#include "ModelManager.h"

int ModelManager::LoadModelOBJ(const std::string &modelname, std::string tag, bool Smoothing)
{
	int check = AlreadyLoadModel(modelname);
	if (check != -1) {
		for (int i = 0; i < models.size(); i++) {
			if (models[i].modelID == check) {
				return models[i].modelID;
			}
		}
	}

	Model model;
	models.emplace_back(model);
	models[models.size() - 1].fileName = modelname;

	HRESULT result;
	models[models.size() - 1].tag = tag;

	const string filename = modelname + ".obj";
	const string directoryPath = "Resource/Model/" + modelname + "/";

	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) {
		assert(0);
	}
	vector<XMFLOAT3> pos;
	vector<XMFLOAT3> normal;
	vector<XMFLOAT2> texcord;
	string line;

	int indexCountTex = 0;

	while (getline(file, line)) {
		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');


		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(&models[models.size() - 1], directoryPath, filename);
		}
		if (key == "v") {
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			pos.emplace_back(position);

		}
		if (key == "vt") {
			XMFLOAT2 texcord2{};
			line_stream >> texcord2.x;
			line_stream >> texcord2.y;
			texcord2.y = 1.0f - texcord2.y;
			texcord.emplace_back(texcord2);
		}
		if (key == "vn") {
			XMFLOAT3 normal2{};
			line_stream >> normal2.x;
			line_stream >> normal2.y;
			line_stream >> normal2.z;
			normal.emplace_back(normal2);
		}
		if (key == "f") {
			int indexCount = 0;

			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexNormal, indexTexcord;
				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex vert{};
				vert.pos = pos[indexPos - 1];
				vert.normal = normal[indexNormal - 1];
				vert.uv = texcord[indexTexcord - 1];
				models[models.size() - 1].vertices.emplace_back(vert);
				//エッジ平滑化用のデータを追加
				if (Smoothing) {
					//vキー(座標データ)の番号と、すべて合成した頂点のインデックスをセットで登録する
					models[models.size() - 1].AddSmoothData(indexPos, (unsigned short)models[models.size() - 1].GetVertexCount() - 1);
				}
				if (indexCount >= 3) {
					models[models.size() - 1].indices.emplace_back(indexCountTex - 1);
					models[models.size() - 1].indices.emplace_back(indexCountTex);
					models[models.size() - 1].indices.emplace_back(indexCountTex - 3);
				}
				else {
					models[models.size() - 1].indices.emplace_back(indexCountTex);
				}
				indexCountTex++;
				indexCount++;
			}
		}
	}
	//各頂点法線の平均によるエッジの平滑化
	if (Smoothing) {
		models[models.size() - 1].CalculateSmoothedVertexNormals();
	}

	models[models.size() - 1]._indices = models[models.size() - 1].indices.size();
	//各頂点について処理
	for (int i = 0; i < models[models.size() - 1].vertices.size(); i++) {
		//最初のボーン(単位行列)の影響100%にする
		models[models.size() - 1].vertices[i].boneIndex[0] = 0;
		models[models.size() - 1].vertices[i].boneWeight[0] = 1.0f;
	}


	models[models.size() - 1].isMaterial = true; //マテリアルを適応する

	file.close();

	int ReturnLoadNum = NowLoadNum;
	NowLoadNum++;
	models[models.size() - 1].modelID = ReturnLoadNum;
	return ReturnLoadNum;
}

int ModelManager::LoadModelFBX(const string &modelName)
{
	int check = AlreadyLoadModel(modelName);
	if (check != -1) {
		for (int i = 0; i < models.size(); i++) {
			if (models[i].modelID == check) {
				return models[i].modelID;
			}
		}
	}

	Model model;
	models.emplace_back(model);
	int Num = models.size() - 1;
	models[Num] = FbxLoader::GetInstance()->LoadModelFromFile(modelName);
	models[Num].fileName = modelName;

	int ReturnLoadNum = NowLoadNum;
	NowLoadNum++;
	models[Num].modelID = ReturnLoadNum;
	return ReturnLoadNum;
}

Model *ModelManager::GetModelData(int modelNum)
{
	if (models.size() > 0) {
		for (int i = 0; i < models.size(); i++) {
			if (models[i].modelID == modelNum) {
				return &models[i];
			}
		}
	}
	return nullptr;
}

void ModelManager::LoadMaterial(Model *model, const std::string &directoryPath, const std::string &filename)
{
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) {
		assert(0);
	}
	string line;
	while (getline(file, line)) {
		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');
		if (key[0] == '\t') {
			key.erase(key.begin());
		}
		if (key == "newmtl") {
			line_stream >> model->material.name;
		}
		if (key == "Ka") {
			line_stream >> model->material.ambient.x;
			line_stream >> model->material.ambient.y;
			line_stream >> model->material.ambient.z;
		}
		if (key == "Kd") {
			line_stream >> model->material.diffuse.x;
			line_stream >> model->material.diffuse.y;
			line_stream >> model->material.diffuse.z;
		}
		if (key == "Ks") {
			line_stream >> model->material.specular.x;
			line_stream >> model->material.specular.y;
			line_stream >> model->material.specular.z;
		}
		if (key == "map_Kd") {
			line_stream >> model->material.textureFilename;
			model->material.texNumber = TexManager::LoadTexture2(directoryPath, model->material.textureFilename);
		}
	}
	file.close();
}

int ModelManager::AlreadyLoadModel(const string &Name)
{
	if (models.size() > 0) {
		for (int i = 0; i < models.size(); i++) {
			if (models[i].fileName == Name) {
				return models[i].modelID;
			}
		}
	}
	return -1;
}
